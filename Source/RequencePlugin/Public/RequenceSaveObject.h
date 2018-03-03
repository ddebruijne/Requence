// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "RequenceSaveObject.generated.h"

USTRUCT(BlueprintType)
struct FRequenceSaveObjectDevice
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)		FString DeviceString = "Unknown";
	UPROPERTY(EditAnywhere, BlueprintReadWrite)		FString DeviceName = "Unknown";
	UPROPERTY(EditAnywhere, BlueprintReadOnly)		ERequenceDeviceType DeviceType = ERequenceDeviceType::RDT_Unknown;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)		TArray<FRequenceInputAction> Actions;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)		TArray<FRequenceInputAxis> Axises;

	FRequenceSaveObjectDevice() {}
};

UCLASS()
class REQUENCEPLUGIN_API URequenceSaveObject : public USaveGame
{
	GENERATED_BODY()

public:
	//Data in saveobject
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)	TArray<FRequenceSaveObjectDevice> Devices;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)	TArray<FString> FullAxisList;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)	TArray<FString> FullActionList;

	//Parameters
	UPROPERTY(VisibleAnywhere, Category = Basic)	FString SaveSlotName;	//todo: add player name
	UPROPERTY(VisibleAnywhere, Category = Basic)	uint32 UserIndex;		//todo: dynamic user slots

	URequenceSaveObject() 
	{
		SaveSlotName = TEXT("RequenceInputSaveObject");
		UserIndex = 0;
	}
	
};
