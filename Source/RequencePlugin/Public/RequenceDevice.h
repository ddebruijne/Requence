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
UCLASS(EditInlineNew, Meta = (BlueprintSpawnableComponent))
class URequenceDevice : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)		FString DeviceString = "Unknown";
	UPROPERTY(EditAnywhere, BlueprintReadOnly)		ERequenceDeviceType DeviceType = ERequenceDeviceType::RDT_Unknown;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)		TArray<FRequenceInputAction> Actions;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)		TArray<FRequenceInputAxis> Axises;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)	bool Updated = false;

	URequenceDevice();

	//Returns the device type by a bound key
	static ERequenceDeviceType GetDeviceTypeByKeyString(FString KeyString);

	//Returns the device name by the set device key. Will return Unknown for unique devices.
	static FString GetDeviceNameByType(ERequenceDeviceType DeviceType);

	//Check whether the provided action is already in the list.
	UFUNCTION() bool HasActionBinding(FString ActionName);

	//Check whether the provided axis is already in the list.
	UFUNCTION() bool HasAxisBinding(FString AxisName);

	//Bubble sorts the actions and axises based on name.
	UFUNCTION() void SortAlphabetically();

	//Adds a new action to this device. Checks for doubles. returns success.
	UFUNCTION() bool AddAction(FRequenceInputAction _action);

	//Adds a new axis to this device. Checks for doubles. returns success.
	UFUNCTION() bool AddAxis(FRequenceInputAxis _axis);

	//Rebinds action.
	UFUNCTION() bool RebindAction(FRequenceInputAction UpdatedAction);

	//Rebinds Axis
	UFUNCTION() bool RebindAxis(FRequenceInputAxis UpdatedAxis);

	//Deletes action by string. returns success.
	UFUNCTION() bool DeleteAction(FString ActionName);

	//Deletes axis by string. returns success.
	UFUNCTION() bool DeleteAxis(FString AxisName);
};