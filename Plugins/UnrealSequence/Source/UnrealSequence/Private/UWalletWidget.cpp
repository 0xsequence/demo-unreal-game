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

#include "SequenceJSLoader.h"

class InternalUWebBrowser : public UWebBrowser
{
public:
    TSharedPtr<SWebBrowser> &GetBrowserWidget()
    {
        if (!WebBrowserWidget.IsValid())
        {
            UE_LOG(LogTemp, Warning, TEXT("[Sequence] getbrowserwidget:INVALID PTR"));
        }
        return WebBrowserWidget;
    }
};

// class InternalSWebBrowser : public SWebBrowser
// {
// public:
//     SWebBrowserView *GetBrowserView()
//     {
//         return BrowserView.Get();
//     }
// };

void UWalletWidget::NativeConstruct()
{
    Super::NativeConstruct();
    DummyWebBrowser->OnBeforePopup.AddDynamic(this, &UWalletWidget::OnCapturePopup);
    auto ChildWebBrowser = static_cast<InternalUWebBrowser *>(DummyWebBrowser);
    auto WebBrowserWidget = ChildWebBrowser->GetBrowserWidget();

    if (WebBrowserWidget.IsValid())
    {
        // WebBrowserWidget->OnLoadStarted.AddDynamic(this, &UWalletWidget::OnLoadStarted);
        WebBrowserWidget->LoadString(SEQUENCE_JS_HTML(), "http://example.com/");
        WebBrowserWidget->BindUObject("walletTransport", this, true);
    }
    // auto addr = std::addressof(this);
    // UE_LOG(LogTemp, Warning, TEXT("address of this: %p, address of webbrwoserwidget: %p"), this, WebBrowserWidget);
    // GEngine->AddOnScreenDebugMessage(0, 200, FColor::Green, TEXT(((void const *)this).str()));
}

void UWalletWidget::OnCapturePopup(FString URL, FString Frame)
{
    UE_LOG(LogTemp, Warning, TEXT("[Sequence] Popup Captured! %s"), *URL);
    WalletWebBrowser->LoadURL(URL);
}

void UWalletWidget::OnLoadStarted()
{
    UE_LOG(LogTemp, Warning, TEXT("[Sequence] Load started!"));
}

void UWalletWidget::SendMesageToWallet(FString JSON)
{
    UE_LOG(LogTemp, Warning, TEXT("[Sequence] Posting message to popup: %s"), *JSON);
    auto HackedWalletWebBrowser = static_cast<InternalUWebBrowser *>(WalletWebBrowser);
    // WebBrowserWidget->
}

// /**
//  * Gets the source of the main frame as raw HTML.
//  *
//  * This method has to be called asynchronously by passing a callback function, which will be called at a later point when the
//  * result is ready.
//  * @param	Callback	A callable that takes a single string reference for handling the result.
//  */
// void GetSource(TFunction<void (const FString&)> Callback) const;

// /**
//  * Expose a UObject instance to the browser runtime.
//  * Properties and Functions will be accessible from JavaScript side.
//  * As all communication with the rendering procesis asynchronous, return values (both for properties and function results) are wrapped into JS Future objects.
//  *
//  * @param Name The name of the object. The object will show up as window.ue4.{Name} on the javascript side. If there is an existing object of the same name, this object will replace it. If bIsPermanent is false and there is an existing permanent binding, the permanent binding will be restored when the temporary one is removed.
//  * @param Object The object instance.
//  * @param bIsPermanent If true, the object will be visible to all pages loaded through this browser widget, otherwise, it will be deleted when navigating away from the current page. Non-permanent bindings should be registered from inside an OnLoadStarted event handler in order to be available before JS code starts loading.
//  */
// void BindUObject(const FString& Name, UObject* Object, bool bIsPermanent = true);