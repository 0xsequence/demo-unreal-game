// Fill out your copyright notice in the Description page of Project Settings.

#include "UWalletWidget.h"
#include "Blueprint/WidgetTree.h"

#include "WebBrowser.h"
#include "SWebBrowser.h"
#include "SWebBrowserView.h"

#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "Internationalization/Text.h"
#include "Widgets/Layout/Anchors.h"
#include "Engine/Engine.h"

DEFINE_LOG_CATEGORY(LogSequence);

class InternalUWebBrowser : public UWebBrowser
{
public:
    TSharedPtr<SWebBrowser> &GetBrowserWidget()
    {
        if (!WebBrowserWidget.IsValid())
        {
            UE_LOG(LogSequence, Fatal, TEXT("getbrowserwidget:INVALID PTR"));
        }
        return WebBrowserWidget;
    }
};

// ALLOW_ACCESS(SWebBrowser, OnLoadCompleted, FSimpleDelegate);

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
    UE_LOG(LogSequence, Log, TEXT("Loading Sequence HTML: %s"), *SequenceHTMLFile);
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
    UE_LOG(LogSequence, Log, TEXT("Loading Sequence JS: %s"), *SequenceJSFile);
    if (!FileManager.FileExists(*SequenceJSFile) || !FFileHelper::LoadFileToString(SequenceJS, *SequenceJSFile, FFileHelper::EHashOptions::None))
    {
        UE_LOG(LogSequence, Fatal, TEXT("Failed to load Sequence JS, can't initialize."));
    }

    FString FullSequenceHTML = LeftSequenceHTML + SequenceJS + RightSequenceHTML;

    DummyWebBrowser->OnBeforePopup.AddDynamic(this, &UWalletWidget::OnCapturePopup);
    auto DummyWebBrowserWidget = (static_cast<InternalUWebBrowser *>(DummyWebBrowser))->GetBrowserWidget();

    DummyWebBrowserWidget->BindUObject("sequencewallettransport", this, true);
    DummyWebBrowserWidget->LoadString(FullSequenceHTML, "http://example.com/");

    auto WalletWebBrowserWidget = (static_cast<InternalUWebBrowser *>(WalletWebBrowser))->GetBrowserWidget();
    WalletWebBrowserWidget->BindUObject("sequencewallettransport", this, true);
}

void UWalletWidget::OnCapturePopup(FString URL, FString Frame)
{
    UE_LOG(LogSequence, Log, TEXT("Popup Captured! %s"), *URL);
    WalletWebBrowser->LoadURL(URL);
}

void UWalletWidget::OnLoadCompleted()
{
    UE_LOG(LogSequence, Log, TEXT("Load Completed!"));
}

void UWalletWidget::SendMessageToWallet(FString JSON)
{
    UE_LOG(LogSequence, Log, TEXT("Posting message to popup: %s"), *JSON);
    auto WalletWebBrowserWidget = (static_cast<InternalUWebBrowser *>(WalletWebBrowser))->GetBrowserWidget();
    WalletWebBrowserWidget->ExecuteJavascript(TEXT("window.ue.sequencewallettransport.onmessagefromsequencejs(" + JSON + ");"));
}

void UWalletWidget::SendMessageToSequenceJS(FString JSON)
{
    UE_LOG(LogSequence, Log, TEXT("Posting message to Sequence JS window: %s"), *JSON);
    auto DummyWebBrowserWidget = (static_cast<InternalUWebBrowser *>(DummyWebBrowser))->GetBrowserWidget();
    DummyWebBrowserWidget->ExecuteJavascript(TEXT("window.ue.sequencewallettransport.onmessagefromwallet(" + JSON + ");"));
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