// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RequenceInputDevice.h"

#include "SDL.h"
#include "SDL_joystick.h"

// #include "RequenceSDL.generated.h"

/**
 * 
 */
// UCLASS()
class REQUENCEPLUGIN_API URequenceSDL /*: public UObject*/
{
// 	GENERATED_BODY()

private:
	TArray<URequenceInputDevice*> Devices;
	bool bOwnsSDL = false;

public:
	URequenceSDL() {}
	~URequenceSDL();
	void Initialize();

	URequenceInputDevice* AddDevice(int DeviceIndex);
	bool RemoveDevice(URequenceInputDevice* DeviceToRemove);

private:
	static int HandleSDLEvent(void* UserData, SDL_Event* Event);
	
};
