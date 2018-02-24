#pragma once

#include "CoreMinimal.h"
#include "GameFramework/InputSettings.h"
#include "RequenceStructs.h"
#include "RequenceDevice.h"
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
class URequence : public UObject
{
	GENERATED_BODY()

public:
	URequence();

	//Data
	UPROPERTY(BlueprintReadOnly)	TArray<FRequenceInputAction>	Actions;	//All Actions (not separated across devices)
	UPROPERTY(BlueprintReadOnly)	TArray<FRequenceInputAxis>		Axises;		//All Axises (not separated across devices)
	UPROPERTY(BlueprintReadOnly)	TArray<URequenceDevice*>		Devices;

	//Core functions
	UFUNCTION(BlueprintCallable)	bool LoadUnrealInput();					//Converts Unreals' Input.ini to our format
	UFUNCTION(BlueprintCallable)	bool SaveUnrealInput(bool Force);		//Places our format safely back as Input.ini

	//Helper Functions
 	UFUNCTION(BlueprintCallable)	URequenceDevice* GetDeviceByName(FString DeviceName);
 	UFUNCTION(BlueprintCallable)	URequenceDevice* GetDeviceByType(ERequenceDeviceType DeviceType);
	UFUNCTION()						URequenceDevice* CreateDevice(FString DeviceName);
	UFUNCTION(BlueprintCallable)	void DebugPrint(bool UseDevices);
	UFUNCTION()						void ClearDevicesAndAxises();
	UFUNCTION(BlueprintCallable)	bool HasUpdated();
};
