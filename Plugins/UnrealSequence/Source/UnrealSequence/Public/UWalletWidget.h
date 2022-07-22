// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WebBrowser.h"
#include "UWalletWidget.generated.h"

UCLASS()
class UNREALSEQUENCE_API UWalletWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly)
	FString WalletURL;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UWebBrowser *DummyWebBrowser;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UWebBrowser *WalletWebBrowser;

private:
	UFUNCTION()
	void OnCapturePopup(FString URL, FString Frame);

	UFUNCTION()
	void OnLoadStarted();
};
