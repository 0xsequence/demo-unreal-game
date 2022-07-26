// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WebBrowser.h"
#include "FConnectOptionsStruct.h"
#include "UWalletWidget.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSequence, Log, All);

UCLASS()
class UNREALSEQUENCE_API UWalletWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// In this context, use the global variable `seq` to access the Sequence SDK.
	UFUNCTION(BlueprintCallable, Category = "Sequence")
	void ExecuteSequenceJS(FString JS);

	UFUNCTION(BlueprintCallable, Category = "Sequence")
	void Connect(FConnectOptionsStruct Options);

	// TODO: expose the whole ProviderConfig struct - probably autogenerate from either SequenceJS code or a proto.ridl file.

	UFUNCTION(BlueprintImplementableEvent)
	void SequencePopupOpened();

	UPROPERTY(EditDefaultsOnly)
	FString DefaultNetwork = "polygon";

	UPROPERTY(EditDefaultsOnly)
	FString WalletAppURL = "https://sequence.app/";

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UWebBrowser *DummyWebBrowser;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UWebBrowser *WalletWebBrowser;

private:
	UFUNCTION()
	void OnCapturePopup(FString URL, FString Frame);

	UFUNCTION()
	void OnLoadCompleted();

	UFUNCTION()
	void SendMessageToSequenceJS(FString message);

	UFUNCTION()
	void SendMessageToWallet(FString message);

	UFUNCTION()
	void LogFromJS(FString Text);
	UFUNCTION()
	void WarnFromJS(FString Text);
	UFUNCTION()
	void ErrorFromJS(FString Text);
};