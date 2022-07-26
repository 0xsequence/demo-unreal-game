// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSettingsStruct.h"
#include "FConnectOptionsStruct.generated.h"

// TODO: Autogenerate this from either SequenceJS code or a proto.ridl file.
USTRUCT(BlueprintType)
struct UNREALSEQUENCE_API FConnectOptionsStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// app name of the dapp which will be announced to user on connect screen
	FString App;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// authorize will perform an ETHAuth eip712 signing and return the proof to the dapp.
	bool Authorize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// Options to further customize the wallet experience.
	FSettingsStruct Settings;
};
