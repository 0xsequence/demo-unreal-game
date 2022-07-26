// Fill out your copyright notice in the Description page of Project Settings.

#include "UWalletWidget.h"
#include "Blueprint/WidgetTree.h"

#include "WebBrowser.h"
#include "SWebBrowser.h"
#include "SWebBrowserView.h"

#include "JsonObjectConverter.h"
#include "Serialization/JsonWriter.h"

#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "Internationalization/Text.h"
#include "Widgets/Layout/Anchors.h"
#include "Engine/Engine.h"

DEFINE_LOG_CATEGORY(LogSequence);

// Begin evil, evil, evil, evil hackery that makes private & protected fields accessible on demand
// This is the only way to accwess the internals of the UE5 WebBrowser stuff they don't expose,
// which we need to write our API.
// ----------------------------------------------------------------
// Copied from https://gist.github.com/dabrahams/1528856
// Generate a static data member of type Tag::type in which to store
// the address of a private member.  It is crucial that Tag does not
// depend on the /value/ of the the stored address in any way so that
// we can access it from ordinary code without directly touching
// private data.
template <class Tag>
struct stowed
{
    static typename Tag::type value;
};

template <class Tag>
typename Tag::type stowed<Tag>::value;

// Generate a static data member whose constructor initializes
// stowed<Tag>::value.  This type will only be named in an explicit
// instantiation, where it is legal to pass the address of a private
// member.
template <class Tag, typename Tag::type x>
class stow_private
{
    stow_private() { stowed<Tag>::value = x; }
    static stow_private instance;
};
template <class Tag, typename Tag::type x>
stow_private<Tag, x> stow_private<Tag, x>::instance;
// End evil, evil, evil hackery that makes private field accessible

// Use Evil Hackery to get a SWebBrowser out of a UWebBrowser
struct UWebBrowser_WebBrowserWidget
{
    typedef TSharedPtr<SWebBrowser>(UWebBrowser::*type);
};
template class stow_private<UWebBrowser_WebBrowserWidget, &UWebBrowser::WebBrowserWidget>;

// Use Evil Hackery to get a SWebBrowserView out of a SWebBrowser
struct SWebBrowser_BrowserView
{
    typedef TSharedPtr<SWebBrowserView>(SWebBrowser::*type);
};
template class stow_private<SWebBrowser_BrowserView, &SWebBrowser::BrowserView>;

// Use Evil Hackery to get an OnLoadCompleted FSimpleDelegate out of a SWebBrowserView
struct SWebBrowserView_OnLoadCompleted
{
    typedef FSimpleDelegate(SWebBrowserView::*type);
};
template class stow_private<SWebBrowserView_OnLoadCompleted, &SWebBrowserView::OnLoadCompleted>;

void UWalletWidget::ExecuteSequenceJS(FString JS)
{
    DummyWebBrowser->ExecuteJavascript(JS);
}

void UWalletWidget::Connect(FConnectOptionsStruct Options)
{
    if (Options.App == "")
    {
        UE_LOG(LogSequence, Error, TEXT("No App name passed to Options of Connect."));
        return;
    }
    // TODO: do JSON in a nicer way than manual string concat.
    FString ConnectOptionsJSON =
        FString("{ app: \"") +
        Options.App +
        FString("\", authorize: ") +
        (Options.Authorize
             ? FString("true")
             : FString("undefined")) +
        FString("}");

    //     settings: {
    //         bannerUrl: 'https://sequence.xyz/built-in-security/animation.webp',
    //     },
    //     origin: undefined

    UE_LOG(LogSequence, Warning, TEXT("APP NAME seq.getWallet().connect(%s);"), *ConnectOptionsJSON)

    DummyWebBrowser->ExecuteJavascript("seq.getWallet().connect(" + ConnectOptionsJSON + ");");
}

void UWalletWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Load data files from .pak
    IPlatformFile &FileManager = FPlatformFileManager::Get().GetPlatformFile();
    auto ThisPluginDir = FPaths::ConvertRelativePathToFull(FPaths::Combine(FPaths::ProjectPluginsDir() + "UnrealSequence/Data"));

    if (!FileManager.DirectoryExists(*ThisPluginDir))
    {
        UE_LOG(LogSequence, Fatal, TEXT("Failed to find Sequence Data folder, can't initialize."));
    }

    FString SequenceHTML;
    FString SequenceHTMLFile = FPaths::Combine(ThisPluginDir + "/sequence.html");
    UE_LOG(LogSequence, Log, TEXT("Loading Sequence HTML from %s"), *SequenceHTMLFile);
    if (!FileManager.FileExists(*SequenceHTMLFile) || !FFileHelper::LoadFileToString(SequenceHTML, *SequenceHTMLFile, FFileHelper::EHashOptions::None))
    {
        UE_LOG(LogSequence, Fatal, TEXT("Failed to load Sequence HTML, can't initialize."));
    }

    FString LeftSequenceHTML;
    FString RightSequenceHTML;

    if (!SequenceHTML.Split(TEXT("/*%SEQUENCE_JS_HERE%*/"), &LeftSequenceHTML, &RightSequenceHTML))
    {
        UE_LOG(LogSequence, Fatal, TEXT("Malformed Sequence HTML, can't initialize. Missing JS placeholder."));
    }

    FString SequenceJS;
    FString SequenceJSFile = FPaths::Combine(ThisPluginDir + "/0xsequence.umd.min.js");
    UE_LOG(LogSequence, Log, TEXT("Loading Sequence JS from %s"), *SequenceJSFile);
    if (!FileManager.FileExists(*SequenceJSFile) || !FFileHelper::LoadFileToString(SequenceJS, *SequenceJSFile, FFileHelper::EHashOptions::None))
    {
        UE_LOG(LogSequence, Fatal, TEXT("Failed to load Sequence JS, can't initialize."));
    }
    FString SequenceJSInit = TEXT(
                                 "</script><script>window.seq = window.sequence.sequence; window.seq.initWallet('") +
                             DefaultNetwork + TEXT("', { walletAppURL:'") +
                             WalletAppURL + TEXT("', transports: { unrealTransport: { enabled: true } } });");

    // Create an HTML file that loads sequence.js
    FString FullSequenceHTML = LeftSequenceHTML + SequenceJS + SequenceJSInit + RightSequenceHTML;

    // Capture popups from the sequence.js code, so we can open a second webview.
    DummyWebBrowser->OnBeforePopup.AddDynamic(this, &UWalletWidget::OnCapturePopup);
    auto DummyWebBrowserWidget = (*DummyWebBrowser).*stowed<UWebBrowser_WebBrowserWidget>::value;

    auto DummyBrowserView = (*DummyWebBrowserWidget).*stowed<SWebBrowser_BrowserView>::value;
    auto DummyBrowserViewOnLoad = (*DummyBrowserView).*stowed<SWebBrowserView_OnLoadCompleted>::value;
    DummyBrowserViewOnLoad.BindUObject(this, &UWalletWidget::OnLoadCompleted);

    // Add our custom Unreal transport to both windows, and load the Sequence.js HTML.
    DummyWebBrowserWidget->BindUObject("sequencewallettransport", this, true);
    DummyWebBrowserWidget->LoadString(FullSequenceHTML, "http://example.com/");

    auto WalletWebBrowserWidget = (*WalletWebBrowser).*stowed<UWebBrowser_WebBrowserWidget>::value;
    WalletWebBrowserWidget->BindUObject("sequencewallettransport", this, true);
}

void UWalletWidget::OnCapturePopup(FString URL, FString Frame)
{
    WalletWebBrowser->LoadURL(URL);
    SequencePopupOpened();
}

void UWalletWidget::OnLoadCompleted()
{
    UE_LOG(LogSequence, Warning, TEXT("OnLoadCompleted called!"));
}

void UWalletWidget::SendMessageToWallet(FString JSON)
{
    WalletWebBrowser->ExecuteJavascript(TEXT("window.ue.sequencewallettransport.onmessagefromsequencejs(" + JSON + ");"));
}

void UWalletWidget::SendMessageToSequenceJS(FString JSON)
{
    DummyWebBrowser->ExecuteJavascript(TEXT("window.ue.sequencewallettransport.onmessagefromwallet(" + JSON + ");"));
}

void UWalletWidget::LogFromJS(FString Text)
{
    UE_LOG(LogSequence, Log, TEXT("Log from JS: %s"), *Text);
}
void UWalletWidget::WarnFromJS(FString Text)
{
    UE_LOG(LogSequence, Warning, TEXT("Warn from JS: %s"), *Text);
}
void UWalletWidget::ErrorFromJS(FString Text)
{
    UE_LOG(LogSequence, Error, TEXT("Error from JS: %s"), *Text);
}