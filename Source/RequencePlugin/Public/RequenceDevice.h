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

	//Returns the device type by a bound key
	static ERequenceDeviceType GetDeviceTypeByKeyString(FString KeyString);

	//Returns the device name by the set device key. Will return Unknown for unique devices.
	static FString GetDeviceNameByType(ERequenceDeviceType DeviceType);

	//Adds a new action to this device. Checks for doubles. returns success.
	bool AddAction(FRequenceInputAction _action);

	//Adds a new axis to this device. Checks for doubles. returns success.
	bool AddAxis(FRequenceInputAxis _axis);

	//Rebinds action by string to a new key. Shift, Ctrl, Alt and Cmd are false by default. returns success.
	bool RebindAction(FString ActionName, FKey NewKey, bool bShift, bool bCtrl, bool bAlt, bool bCmd);

	//Rebinds axis by string to a new key. Scale is 1.0 by default. returns success.
	bool RebindAxis(FString AxisName, FKey NewKey, float Scale);

	//Deletes action by string. returns success.
	bool DeleteAction(FString ActionName);

	//Deletes axis by string. returns success.
	bool DeleteAxis(FString ActionName);
};