// Fill out your copyright notice in the Description page of Project Settings.

#include "RequenceSDL.h"

URequenceSDL::~URequenceSDL()
{
	UE_LOG(LogTemp, Log, TEXT("RequenceSDL deconstructing"));

	//Remove all devices
	for (int i = Devices.Num(); i >= 0; i--) {
		//RemoveDevice(InputDevices[i].Name);
	}

	SDL_DelEventWatch(HandleSDLEvent, this);
	if (bOwnsSDL) { SDL_Quit(); }
}

void URequenceSDL::Initialize()
{
	UE_LOG(LogTemp, Log, TEXT("RequenceSDL starting"));

	if (SDL_WasInit(0) != 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("SDL already loaded!"));
		bOwnsSDL = false;
	}
	else
	{
		SDL_Init(0);
		bOwnsSDL = true;
		UE_LOG(LogTemp, Log, TEXT("Took ownership of SDL"));
	}

	if (SDL_InitSubSystem(SDL_INIT_JOYSTICK) == 0)
	{
		UE_LOG(LogTemp, Log, TEXT("Initialized Joystick subsystem"));
	}

	SDL_AddEventWatch(HandleSDLEvent, this);
}

URequenceInputDevice* URequenceSDL::AddDevice(int DeviceIndex)
{
	return nullptr;
}

bool URequenceSDL::RemoveDevice(URequenceInputDevice* DeviceToRemove)
{
	return false;
}

int URequenceSDL::HandleSDLEvent(void* UserData, SDL_Event* Event)
{
	URequenceSDL& Self = *static_cast<URequenceSDL*>(UserData);

	switch (Event->type)
	{
		case SDL_JOYDEVICEADDED:
			UE_LOG(LogTemp, Log, TEXT("Joystick connected"));
			//Self.AddDevice(Event->cdevice.which);
			break;
		case SDL_JOYDEVICEREMOVED:
			UE_LOG(LogTemp, Log, TEXT("Joystick disconnected"));
			break;
	}
	return 0;
}
