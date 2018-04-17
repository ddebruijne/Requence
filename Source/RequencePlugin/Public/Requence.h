#pragma once

#include "CoreMinimal.h"
#include "GameFramework/InputSettings.h"
#include "Engine.h"
#include "RequenceStructs.h"
#include "RequenceDevice.h"
#include "RequenceSaveObject.h"
#include "Paths.h"
#include "Requence.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRequenceOnEditModeStarted, ERequenceDeviceType, DeviceType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRequenceOnEditModeEnded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRequenceUpdatedUniqueDevices);

/*
*  Impeller Studios (2018)
*  Requence
*
*  Input system back-end created for Starfighter inc.
*  Documentation: https://goo.gl/BzAUdq
*
*  Dependencies:
*  - JoystickPlugin 
*/
UCLASS(EditInlineNew, Meta=(BlueprintSpawnableComponent))
class REQUENCEPLUGIN_API URequence : public UObject
{
	GENERATED_BODY()
private:
	//Version of Requence. If this number is different than it is in the save file, it will clear the save to ensure compatibility.
	UPROPERTY()						int Version = 1;

public:
	URequence();
	~URequence();

	//////////////////////////////////////////////////////////////////////////
	// Data
	//////////////////////////////////////////////////////////////////////////

	//All loaded actions
	UPROPERTY(BlueprintReadOnly)	TArray<FRequenceInputAction>	Actions;
private:
	//All Unreal loaded axises. Don't use this por favor.
	UPROPERTY()						TArray<FRequenceInputAxis>	Axises;		

	//All Unreal loaded actions. Don't use this por favor.
	UPROPERTY()						TArray<URequenceDevice*>	Devices;
public:
	//Full Axis list - Used to make sure all devices have every axis
	UPROPERTY()						TArray<FString> FullAxisList;

	//Full Action list - Used to make sure all devices have every action
	UPROPERTY()						TArray<FString> FullActionList;

	//Whether edit mode is enabled - This can be used in blueprint to track if one entry is being edited.
	UPROPERTY()						bool bEditModeEnabled = false;

	//If edit mode is enabled, this will contain the device type.
	UPROPERTY()						ERequenceDeviceType EditModeDeviceType = ERequenceDeviceType::RDT_Unknown;


	//////////////////////////////////////////////////////////////////////////
	// Delegates
	//////////////////////////////////////////////////////////////////////////
public:
	//Called when Edit Mode has started
	UPROPERTY(BlueprintAssignable)	FRequenceOnEditModeStarted OnEditModeStarted;
	
	//Called when Edit Mode has ended
	UPROPERTY(BlueprintAssignable)	FRequenceOnEditModeEnded OnEditModeEnded;

	//Called when we've updated our RID's - Update your unique devices UI when this is called. (eg connection/disconnection and name change.)
	UPROPERTY(BlueprintAssignable)	FRequenceUpdatedUniqueDevices OnUniqueDevicesUpdated;


	//////////////////////////////////////////////////////////////////////////
	// Core functions
	//////////////////////////////////////////////////////////////////////////
private:
	//Converts Unreals' Input.ini to our format. Returns false if failed.
	UFUNCTION()						bool LoadUnrealInput();	

	//Places our format safely back as Input.ini. Returns false if failed. Note that this function should be avoided and use the Requence save file.
	UFUNCTION()						bool SaveUnrealInput(bool Force);		

	//Delegate callback for when RID devices are updated.
	UFUNCTION()						void RequenceInputDevicesUpdated();
public:
	//Load in Requence save file. If it does not exist (since nothing is customized) we load in UE4's Defaults. Returns success. If ForceDefault is true all devices will be reset to their default bindings.
	UFUNCTION(BlueprintCallable)	bool LoadInput(bool ForceDefault);

	//Save requence save file. Returns success.
	UFUNCTION(BlueprintCallable)	bool SaveInput();

	//Apply axises and actions set in Devices to the temporary UE4 input so we can use them in this session.
	UFUNCTION(BlueprintCallable)	bool ApplyAxisesAndActions(bool Force);

	//Function to run on game startup. Loads in Save game and applies custom inputs to runtime.
	UFUNCTION(BlueprintCallable)	void OnGameStartup();


	//////////////////////////////////////////////////////////////////////////
	//Importing / Exporting 
	//////////////////////////////////////////////////////////////////////////

	//Exports a given device to JSON format.
	UFUNCTION(BlueprintCallable)	void ExportDeviceAsPreset(URequenceDevice* Device);	
	
	// Imports a JSON file and creates a device. returns false if failed.
	UFUNCTION(BlueprintCallable)	bool ImportDeviceAsPreset(FString AbsolutePath);

	//Returns a list of filenames that can be imported (in the default folder). Empty if failed.
	UFUNCTION(BlueprintCallable)	TArray<FString> GetImportableDevicePresets();	


	//////////////////////////////////////////////////////////////////////////
	//Helper Functions
	//////////////////////////////////////////////////////////////////////////

	//Fills full axis/action list from save file.
	UFUNCTION()						bool FillFullAxisActionLists();

	//Tries to find a device by its DeviceString. Returns a nullptr when failed.
 	UFUNCTION(BlueprintCallable)	URequenceDevice* GetDeviceByString(FString DeviceString);

	//Tries to find a device by its ERequenceDeviceType. Returns a nullptr when failed.
 	UFUNCTION(BlueprintCallable)	URequenceDevice* GetDeviceByType(ERequenceDeviceType DeviceType);

	//Tries to create a device by a key name. Returns a nullptr when failed.
	UFUNCTION()						URequenceDevice* CreateDevice(FString KeyName);

	//Prints all found axises and actions.
	UFUNCTION(BlueprintCallable)	void DebugPrint(bool UseDevices);

	//Clears all devices, axises and actions.
	UFUNCTION(BlueprintCallable)	void ClearDevicesAndAxises();

	//Check whether one of our actions/axises were updated
	UFUNCTION(BlueprintCallable)	bool HasUpdated();

	//Returns the default import/export path for input presets.
	UFUNCTION(BlueprintCallable)	FString GetDefaultPresetFilePath() { return FPaths::GameSavedDir() + "InputPresets/"; }

	//Returns whether edit mode is enabled, if enabled it will also return the device type. if not device type is null.
	UFUNCTION(BlueprintCallable)	bool GetEditModeEnabled(ERequenceDeviceType& DeviceType);

	//Starts edit mode.
	UFUNCTION(BlueprintCallable)	void SetEditModeStarted(ERequenceDeviceType DeviceType);

	//Ends edit mode.
	UFUNCTION(BlueprintCallable)	void SetEditModeEnded();
};
