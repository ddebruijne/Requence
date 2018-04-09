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

	for (FSDLDeviceInfo Device : Devices) {
		if (Device.Joystick != nullptr) {
			SDL_JoystickClose(Device.Joystick);
		}
	}
	Devices.Empty();

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
			Self.AddDevice(Event->jdevice.which);
			break;
		case SDL_JOYDEVICEREMOVED:
			Self.RemDevice(Event->jdevice.which);
			break;
		case SDL_JOYBUTTONDOWN:
			break;
		case SDL_JOYBUTTONUP:
			break;
		default:
			break;
	}

	return 0;
}

bool RequenceInputDevice::AddDevice(int Which)
{
	//It's already in!
	for (FSDLDeviceInfo d : Devices) {
		if (d.Which == Which) { return false; }
	}

	FSDLDeviceInfo Device;
	Device.Which = Which;
	Device.Joystick = SDL_JoystickOpen(Which);
	if (Device.Joystick == nullptr) {
		return false;
	}
	Device.InstanceID = SDL_JoystickInstanceID(Device.Joystick);

	Device.Name = FString(ANSI_TO_TCHAR(SDL_JoystickName(Device.Joystick)));
	UE_LOG(LogTemp, Log, TEXT("Requence input device connected: %s (which: %i, instance: %i)"), *Device.Name, Which, Device.InstanceID);
	UE_LOG(LogTemp, Log, TEXT("- Axises %i"), SDL_JoystickNumAxes(Device.Joystick));
	UE_LOG(LogTemp, Log, TEXT("- Balls %i"), SDL_JoystickNumBalls(Device.Joystick));
	UE_LOG(LogTemp, Log, TEXT("- Buttons %i"), SDL_JoystickNumButtons(Device.Joystick));
	UE_LOG(LogTemp, Log, TEXT("- Hats %i"), SDL_JoystickNumHats(Device.Joystick));

	Devices.Add(Device);
	return true;
}

bool RequenceInputDevice::RemDevice(int InstanceID)
{
	bool found = false;
	for (int i = Devices.Num()-1; i >= 0; i--) {
		if (Devices[i].InstanceID == InstanceID) {
			found = true;
			UE_LOG(LogTemp, Log, TEXT("Requence input device disconnected: %s"), *Devices[i].Name);

			if (Devices[i].Joystick != nullptr)
			{
				SDL_JoystickClose(Devices[i].Joystick);
				Devices[i].Joystick = nullptr;
			}
			else { UE_LOG(LogTemp, Warning, TEXT("Tried to remove %s but the SDL device was a nullpointer! Cleaning up..."), *Devices[i].Name); }

			Devices.RemoveAt(i);
			break;
		}
	}

	//return success.
	return !found;
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
