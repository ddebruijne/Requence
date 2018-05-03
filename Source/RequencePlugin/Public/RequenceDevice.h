// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RequenceStructs.h"
#include "RequenceSaveObject.h"
#include "JsonValue.h"
#include "RequenceDevice.generated.h"

class URequence;

/*
*  Danny de Bruijne (2018)
*  RequenceDevice
*  
*  Devices supported by requence containing key rebind functionality.
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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)	bool Connected = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)	URequence* RequenceRef;

	URequenceDevice();

	//Returns the device type by a bound key
	static ERequenceDeviceType GetDeviceTypeByKeyString(FString KeyString);
	UFUNCTION(BlueprintCallable) ERequenceDeviceType GetDeviceTypeByKey(FKey Key);

	//Returns the device name by the set device key. Will return Unknown for unique devices.
	static FString GetDeviceNameByType(ERequenceDeviceType DeviceType);

	//Loops through all axises and actions and applies the CompactifyName function on the Action/Axis name.
	UFUNCTION() void CompactifyAllKeyNames();

	//Returns a new struct with updated KeyString
	UFUNCTION(BlueprintCallable) FRequenceInputAction UpdateKeyStringAction(FRequenceInputAction Action, bool bDoCompactify);

	//Returns a new struct with updated KeyString
	UFUNCTION(BlueprintCallable) FRequenceInputAxis UpdateKeyStringAxis(FRequenceInputAxis Axis, bool bDoCompactify);

	//Generates a new keystring axis
	UFUNCTION(BlueprintCallable) FString GenerateKeyString(FKey key, bool bDoCompactify);

	//Filters the name of an key so it's more compact.
	UFUNCTION(BlueprintCallable) FString CompactifyKeyString(FString InName);

	//Bubble sorts the actions and axises based on name.
	UFUNCTION(BlueprintCallable) void SortAlphabetically();

	//Starts edit mode for this device. returns success.
	UFUNCTION(BlueprintCallable) bool StartEditMode();

	//Adds all not-found axises and actions from the full action list.
	UFUNCTION()	void AddAllEmpty(TArray<FString> FullAxisList, TArray<FString> FullActionList, int numRequired = 2);

	//Removes all actions and axises that are no longer found in the full action/axis list. returns whether axises are removed.
	UFUNCTION() bool FilterDeleted(TArray<FString> FullAxisList, TArray<FString> FullActionList);


	//////////////////////////////////////////////////////////////////////////
	// Axis/Action manipulation
	//////////////////////////////////////////////////////////////////////////

	//Check whether the provided action is already in the list.
	UFUNCTION(BlueprintCallable) bool HasActionBinding(FString ActionName, bool MustBeBound);

	//Check whether the provided axis is already in the list.
	UFUNCTION(BlueprintCallable) bool HasAxisBinding(FString AxisName, bool MustBeBound);

	//Check how many of one action binding are already in the list.
	UFUNCTION(BlueprintCallable) int HasNumOfActionBinding(FString ActionName, bool bMustBeBound);

	//Check how many of one axis binding are already in the list.
	UFUNCTION(BlueprintCallable) int HasNumOfAxisBinding(FString AxisName, bool bMustBeBound);

	//Adds a new action to this device. Checks for doubles. returns success.
	UFUNCTION(BlueprintCallable) bool AddAction(FRequenceInputAction _action);

	//Adds a new axis to this device. Checks for doubles. returns success.
	UFUNCTION(BlueprintCallable) bool AddAxis(FRequenceInputAxis _axis);

	//Rebinds action. ActionName must be the same. returns success.
	UFUNCTION(BlueprintCallable) bool RebindAction(FRequenceInputAction OldAction, FRequenceInputAction UpdatedAction);

	//Rebinds Axis. AxisName must be the same. returns success.
	UFUNCTION(BlueprintCallable) bool RebindAxis(FRequenceInputAxis OldAxis, FRequenceInputAxis UpdatedAxis);

	//Deletes action by string. returns success. bLeaveActionName leaves your struct in the array if you don't want to create news but rebind
	UFUNCTION(BlueprintCallable) bool DeleteActionKeys(FString ActionName);

	//Deletes axis by string. returns success. bLeaveAxisName leaves your struct in the array if you don't want to create news but rebind
	UFUNCTION(BlueprintCallable) bool DeleteAxisKeys(FString AxisName);


	//////////////////////////////////////////////////////////////////////////
	// JSON Import/Export
	//////////////////////////////////////////////////////////////////////////

	//Fills this device from a requence save object device.
	void FromStruct(FRequenceSaveObjectDevice StructIn, URequence* _RequenceRef,
		TArray<FString> FullAxisList, TArray<FString> FullActionList);

	//Creates a save object device from this device.
	FRequenceSaveObjectDevice ToStruct();

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