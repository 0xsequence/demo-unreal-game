// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WebBrowser.h"
#include "UWalletWidget.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSequence, Log, All);

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

// Cursed hack
#define CONCATE_(X, Y) X##Y
#define CONCATE(X, Y) CONCATE_(X, Y)

#define ALLOW_ACCESS(CLASS, MEMBER, ...)                             \
	template <typename Only, __VA_ARGS__ CLASS::*Member>             \
	struct CONCATE(MEMBER, __LINE__)                                 \
	{                                                                \
		friend __VA_ARGS__ CLASS::*Access(Only *) { return Member; } \
	};                                                               \
	template <typename>                                              \
	struct Only_##MEMBER;                                            \
	template <>                                                      \
	struct Only_##MEMBER<CLASS>                                      \
	{                                                                \
		friend __VA_ARGS__ CLASS::*Access(Only_##MEMBER<CLASS> *);   \
	};                                                               \
	template struct CONCATE(MEMBER, __LINE__)<Only_##MEMBER<CLASS>, &CLASS::MEMBER>

#define ACCESS(OBJECT, MEMBER) \
	(OBJECT).*Access((Only_##MEMBER<std::remove_reference<decltype(OBJECT)>::type> *)nullptr)