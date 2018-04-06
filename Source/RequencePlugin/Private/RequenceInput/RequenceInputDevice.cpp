// Fill out your copyright notice in the Description page of Project Settings.

#include "RequenceInputDevice.h"

RequenceInputDevice::RequenceInputDevice(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler) : MessageHandler(InMessageHandler)
{
	InitSDL();
}

RequenceInputDevice::~RequenceInputDevice()
{
	UE_LOG(LogTemp, Log, TEXT("Quitting SDL."));

	SDL_DelEventWatch(HandleSDLEvent, this);

	if (bOwnsSDL)
	{
		SDL_Quit();
	}
}

void RequenceInputDevice::InitSDL()
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

int RequenceInputDevice::HandleSDLEvent(void* UserData, SDL_Event* Event)
{
	RequenceInputDevice& Self = *static_cast<RequenceInputDevice*>(UserData);

	switch (Event->type) 
	{
		case SDL_JOYDEVICEADDED:
			UE_LOG(LogTemp, Log, TEXT("SDL DEVICE ADD EVENT"));
			break;
		case SDL_JOYDEVICEREMOVED:
			UE_LOG(LogTemp, Log, TEXT("SDL DEVICE REM EVENT"));
			break;
		default:
			break;
	}

	return 0;
}

void RequenceInputDevice::Tick(float DeltaTime)
{

}

void RequenceInputDevice::SendControllerEvents()
{
	if (bOwnsSDL)
	{
		SDL_Event Event;
		while (SDL_PollEvent(&Event)) {}
	}
}

void RequenceInputDevice::SetMessageHandler(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler)
{
}

bool RequenceInputDevice::Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar)
{
	return false;
}

void RequenceInputDevice::SetChannelValue(int32 ControllerId, FForceFeedbackChannelType ChannelType, float Value)
{
}

void RequenceInputDevice::SetChannelValues(int32 ControllerId, const FForceFeedbackValues &values)
{
}
