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
	UPROPERTY()		FString DeviceString = "Unknown";
	UPROPERTY()		FString DeviceName = "Unknown";
	UPROPERTY()		ERequenceDeviceType DeviceType = ERequenceDeviceType::RDT_Unknown;
	UPROPERTY()		TArray<FRequenceInputAction> Actions;	//Note: filtered without empty actions. 
	UPROPERTY()		TArray<FRequenceInputAxis> Axises;		//Note: filtered without empty actions.

	FRequenceSaveObjectDevice() {}
};

UCLASS()
class REQUENCEPLUGIN_API URequenceSaveObject : public USaveGame
{
	GENERATED_BODY()

public:
	//Data in saveobject
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)	TArray<FRequenceSaveObjectDevice> Devices;

	//Parameters
	UPROPERTY(VisibleAnywhere, Category = Basic)	FString SaveSlotName;	//todo: add player name
	UPROPERTY(VisibleAnywhere, Category = Basic)	uint32 UserIndex;		//todo: dynamic user slots

	URequenceSaveObject() 
	{
		SaveSlotName = TEXT("RequenceInputSaveObject");
		UserIndex = 0;
	}
	
};
