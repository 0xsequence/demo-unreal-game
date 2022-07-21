// Fill out your copyright notice in the Description page of Project Settings.

#include "UWalletWidget.h"
#include "Blueprint/WidgetTree.h"
#include "WebBrowser.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "Internationalization/Text.h"
#include "Widgets/Layout/Anchors.h"
#include "Engine/Engine.h"

#include "SequenceJSLoader.h"

class Hack : public UWebBrowser
{
public:
    SWebBrowser *GetBrowserWidget()
    {
        return WebBrowserWidget.Get();
    }
};

void UWalletWidget::NativeConstruct()
{
    Super::NativeConstruct();
    DummyWebBrowser->OnBeforePopup.AddDynamic(this, &UWalletWidget::OnCapturePopup);
    auto ChildWebBrowser = reinterpret_cast<Hack *>(&DummyWebBrowser);
    auto WebBrowserWidget = ChildWebBrowser->GetBrowserWidget();
    if (WebBrowserWidget)
    {
        // now, use this to create a transport object in the window scope
        // wtf->BindUObject()
        GEngine->AddOnScreenDebugMessage(2, 20, FColor::Green, TEXT("HOLY SHIT???"));
    }
    DummyWebBrowser->LoadString(SEQUENCE_JS_HTML(), "http://example.com/");
}

void UWalletWidget::OnCapturePopup(FString URL, FString Frame)
{
    GEngine->AddOnScreenDebugMessage(0, 20, FColor::Green, TEXT("Popup Captured!"));
    GEngine->AddOnScreenDebugMessage(1, 20, FColor::Green, URL);
    WalletWebBrowser->LoadURL(URL);
}
