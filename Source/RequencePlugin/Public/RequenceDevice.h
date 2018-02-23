// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RequenceStructs.h"
#include "RequenceDevice.generated.h"

/*
*  Impeller Studios (2018)
*  RequenceDevice
*  Contributors: Danny de Bruijne
*
*  ...
*/
UCLASS()
class URequenceDevice : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	FString DeviceString = "Unknown";
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	ERequenceDeviceType DeviceType = ERequenceDeviceType::RDT_Unknown;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	TArray<FRequenceInputAction> Actions;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)	TArray<FRequenceInputAxis> Axises;

	URequenceDevice();

	static ERequenceDeviceType GetDeviceTypeByKeyString(FString KeyString);
	static FString GetDeviceNameByType(ERequenceDeviceType DeviceType);
	bool AddAction(FRequenceInputAction _action);
	bool AddAxis(FRequenceInputAxis _axis);
};