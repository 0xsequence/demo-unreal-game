// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FSettingsStruct.generated.h"

// TODO: Autogenerate this from either SequenceJS code or a proto.ridl file.
USTRUCT(BlueprintType)
struct UNREALSEQUENCE_API FSettingsStruct
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    // Specify a banner image URL. This image, if provided, will be displayed on the wallet during the connect/authorize process
    FString BannerUrl;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    // Specify a wallet theme. light and dark are the main themes, to use other available themes, you can use the camel case version of the theme names in the wallet settings. For example: "Blue Dark" on wallet UI can be passed as "blueDark". Note that this setting will not be persisted, use wallet.open with 'openWithOptions' intent to set when you open the wallet for user.
    FString Theme;
};
