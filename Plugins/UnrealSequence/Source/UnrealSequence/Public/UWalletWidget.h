// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WebBrowser.h"
#include "FConnectOptionsStruct.h"
#include "UWalletWidget.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSequence, Log, All);

// DECLARE_DYNAMIC_DELEGATE(FJSCallback);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSequenceWalletStateEvent);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnSequenceJSCallback, FString, JSReturnValue);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnSequenceIsConnectedCallback, bool, IsConnected);

UCLASS()
class UNREALSEQUENCE_API UWalletWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// In this context, use the global variable `seq` to access the Sequence SDK, and `ethers` to access ethers.js.
	UFUNCTION(BlueprintCallable, Category = "Sequence")
	void ExecuteSequenceJS(FString JS);

	// In this context, use the global variable `seq` to access the Sequence SDK, and `ethers` to access ethers.js. You MUST call the method "cb" to return a string, or you'll leak memory.
	UFUNCTION(BlueprintCallable, Category = "Sequence")
	void ExecuteSequenceJSWithCallback(FString JS, const FOnSequenceJSCallback &Callback);

	// Convenience method to connect without writing JS
	UFUNCTION(BlueprintCallable, Category = "Sequence")
	void Connect(FConnectOptionsStruct Options, const FOnSequenceJSCallback &Callback);

	// Convenience method to check connection status without writing JS
	UFUNCTION(BlueprintCallable, Category = "Sequence")
	void GetIsConnected(const FOnSequenceIsConnectedCallback &Callback);

	// Convenience method to open the wallet without writing JS
	UFUNCTION(BlueprintCallable, Category = "Sequence")
	void OpenWallet();

	/* Called when the Sequence Wallet window opens. */
	UPROPERTY(BlueprintAssignable, Category = "Sequence|Event")
	FOnSequenceWalletStateEvent OnSequenceWalletPopupOpened;

	/* Called when the Sequence Wallet window closes. */
	UPROPERTY(BlueprintAssignable, Category = "Sequence|Event")
	FOnSequenceWalletStateEvent OnSequenceWalletPopupClosed;

	/* Called when the Sequence Wallet is initializd and ready to take JS commands. */
	UPROPERTY(BlueprintAssignable, Category = "Sequence|Event")
	FOnSequenceWalletStateEvent OnSequenceInitialized;

	// TODO: expose the whole ProviderConfig struct - probably autogenerate from either SequenceJS code or a proto.ridl file.

	UPROPERTY(EditAnywhere, Category = "Sequence")
	FString DefaultNetwork = "polygon";

	UPROPERTY(EditAnywhere, Category = "Sequence")
	FString WalletAppURL = "https://sequence.app/";

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UWebBrowser *DummyWebBrowser;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UWebBrowser *WalletWebBrowser;

private:
	uint32 LastJSCallbackKey = 1;
	TMap<uint32, TFunction<void(FString)>> JSCallbackMap;

	// In this context, use the global variable `seq` to access the Sequence SDK. You MUST call the method "cb" to return a string, or you'll leak memory.
	void InternalExecuteSequenceJSWithCallback(FString JS, TFunction<void(FString)> Callback);

	UFUNCTION()
	void OnCapturePopup(FString URL, FString Frame);

	UFUNCTION()
	void SendMessageToSequenceJS(FString message);

	UFUNCTION()
	void SendMessageToWallet(FString message);

	UFUNCTION()
	void CallbackFromJS(uint32 Key, FString Value);

	UFUNCTION()
	void LogFromJS(FString Text);
	UFUNCTION()
	void WarnFromJS(FString Text);
	UFUNCTION()
	void ErrorFromJS(FString Text);
};