#pragma once

#include "CoreMinimal.h"
#include "GameFramework/InputSettings.h"
#include "Engine.h"
#include "RequenceStructs.h"
#include "RequenceDevice.h"
#include "RequenceSaveObject.h"
#include "Paths.h"
#include "Requence.generated.h"

/*
*  Impeller Studios (2018)
*  Requence
*  Contributors: Danny de Bruijne
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

public:
	URequence();

	//////////////////////////////////////////////////////////////////////////
	// Data
	//////////////////////////////////////////////////////////////////////////

	//All loaded actions
	UPROPERTY(BlueprintReadOnly)	TArray<FRequenceInputAction>	Actions;
private:
	//All Unreal loaded axises. Don't use this por favor.
	UPROPERTY()						TArray<FRequenceInputAxis>	Axises;		
	UPROPERTY()						TArray<URequenceDevice*>	Devices;
public:
	UPROPERTY(BlueprintReadOnly)	TArray<FString> FullAxisList;
	UPROPERTY(BlueprintReadOnly)	TArray<FString> FullActionList;


	//////////////////////////////////////////////////////////////////////////
	//Core functions
	//////////////////////////////////////////////////////////////////////////

	//Converts Unreals' Input.ini to our format. Returns false if failed.
	UFUNCTION()						bool LoadUnrealInput();	

	//Places our format safely back as Input.ini. Returns false if failed. Note that this function should be avoided and use the Requence save file.
	UFUNCTION()						bool SaveUnrealInput(bool Force);		

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

	//Tries to find a device by its DeviceString. Returns a nullptr when failed.
 	UFUNCTION(BlueprintCallable)	URequenceDevice* GetDeviceByString(FString DeviceString);

	//Tries to find a device by its ERequenceDeviceType. Returns a nullptr when failed.
 	UFUNCTION(BlueprintCallable)	URequenceDevice* GetDeviceByType(ERequenceDeviceType DeviceType);

	//Tries to create a device by a key name. Returns a nullptr when failed.
	UFUNCTION()						URequenceDevice* CreateDevice(FString KeyName);

	//Add empty bindings so this device has all the mappings.
	UFUNCTION()						void AddAllEmpty(URequenceDevice* Device);

	//Prints all found axises and actions.
	UFUNCTION(BlueprintCallable)	void DebugPrint(bool UseDevices);

	//Clears all devices, axises and actions.
	UFUNCTION(BlueprintCallable)	void ClearDevicesAndAxises();

	//Check whether one of our actions/axises were updated
	UFUNCTION(BlueprintCallable)	bool HasUpdated();

	//Returns the default import/export path for input presets.
	UFUNCTION(BlueprintCallable)	FString GetDefaultPresetFilePath() { return FPaths::GameSavedDir() + "InputPresets/"; }
};
