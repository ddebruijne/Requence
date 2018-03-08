// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RequenceStructs.h"
#include "RequenceSaveObject.h"
#include "JsonValue.h"
#include "RequenceDevice.generated.h"

/*
*  Impeller Studios (2018)
*  RequenceDevice
*  Contributors: Danny de Bruijne
*
*  ...
*/
UCLASS(EditInlineNew, Meta = (BlueprintSpawnableComponent))
class REQUENCEPLUGIN_API URequenceDevice : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)		FString DeviceString = "Unknown";
	UPROPERTY(EditAnywhere, BlueprintReadWrite)		FString DeviceName = "Unknown";
	UPROPERTY(EditAnywhere, BlueprintReadOnly)		ERequenceDeviceType DeviceType = ERequenceDeviceType::RDT_Unknown;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)		TArray<FRequenceInputAction> Actions;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)		TArray<FRequenceInputAxis> Axises;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)	bool Updated = false;

	URequenceDevice();

	void FromStruct(FRequenceSaveObjectDevice StructIn);

	FRequenceSaveObjectDevice ToStruct();

	//Returns the device type by a bound key
	static ERequenceDeviceType GetDeviceTypeByKeyString(FString KeyString);

	//Returns the device name by the set device key. Will return Unknown for unique devices.
	static FString GetDeviceNameByType(ERequenceDeviceType DeviceType);

	//Loops through all axises and actions and applies the CompactifyName function on the Action/Axis name.
	UFUNCTION() void CompactifyAllKeyNames();

	//Filters the name of an key so it's more compact.
	UFUNCTION() FString CompactifyKeyName(FString InName);

	//Check whether the provided action is already in the list.
	UFUNCTION(BlueprintCallable) bool HasActionBinding(FString ActionName, bool MustBeBound);

	//Check whether the provided axis is already in the list.
	UFUNCTION(BlueprintCallable) bool HasAxisBinding(FString AxisName, bool MustBeBound);

	//Bubble sorts the actions and axises based on name.
	UFUNCTION(BlueprintCallable) void SortAlphabetically();

	//Adds a new action to this device. Checks for doubles. returns success.
	UFUNCTION() bool AddAction(FRequenceInputAction _action);

	//Adds a new axis to this device. Checks for doubles. returns success.
	UFUNCTION() bool AddAxis(FRequenceInputAxis _axis);

	//Rebinds action. ActionName must be the same. returns success.
	UFUNCTION(BlueprintCallable) bool RebindAction(FRequenceInputAction UpdatedAction);

	//Rebinds Axis. AxisName must be the same. returns success.
	UFUNCTION(BlueprintCallable) bool RebindAxis(FRequenceInputAxis UpdatedAxis);

	//Deletes action by string. returns success.
	UFUNCTION() bool DeleteAction(FString ActionName);

	//Deletes axis by string. returns success.
	UFUNCTION() bool DeleteAxis(FString AxisName);

	//Retrieves this class' data as a JSON object.
	TSharedPtr<FJsonObject> GetDeviceAsJson();

	//Retrieves action bindings as JSON array
	TArray<TSharedPtr<FJsonValue>> GetActionsAsJson();

	//Retrieves axis bindings as JSON array
	TArray<TSharedPtr<FJsonValue>> GetAxisesAsJson();

	//Parses and adds actions in JSON format to this device.
	void SetJsonAsActions(TArray<TSharedPtr<FJsonValue>> _Actions);

	//Parses and adds axises in JSON format to this device.
	void SetJsonAsAxises(TArray<TSharedPtr<FJsonValue>> _Axises);

};