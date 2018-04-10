// Fill out your copyright notice in the Description page of Project Settings.

#include "RequenceInputDevice.h"
#include "Text.h"

#define LOCTEXT_NAMESPACE "RequencePlugin"

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

	for (int i = 0; i < SDL_NumJoysticks(); i++) 
	{
		AddDevice(i);
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
		case SDL_JOYHATMOTION:
			Self.HandleInput_Hat(Event);
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
	UE_LOG(LogTemp, Log, TEXT("- Buttons %i"), SDL_JoystickNumButtons(Device.Joystick));
	UE_LOG(LogTemp, Log, TEXT("- Hats %i"), SDL_JoystickNumHats(Device.Joystick));

	//Add Buttons
	for (int i = 0; i < SDL_JoystickNumButtons(Device.Joystick); i++)
	{
		FString keyName = FString::Printf(TEXT("Joystick %s Button %i"), *Device.Name, i);
		FKey key{ *keyName };
		Device.Buttons.Add(key);

		//Add a new key if this one isn't there yet.
		if (!EKeys::GetKeyDetails(key).IsValid())
		{
			FText textValue = FText::Format(LOCTEXT("DeviceHat", "Joystick {0} Button {1}"), FText::FromString(Device.Name), FText::AsNumber(i));
			EKeys::AddKey(FKeyDetails(key, textValue, FKeyDetails::GamepadKey));
		}
	}

	//Add Axises
	for (int i = 0; i < SDL_JoystickNumAxes(Device.Joystick); i++)
	{
		FString keyName = FString::Printf(TEXT("Joystick %s Axis %i"), *Device.Name, i);
		FKey key{ *keyName };
		Device.Axises.Add(key);

		//Add a new key if this one isn't there yet.
		if (!EKeys::GetKeyDetails(key).IsValid())
		{
			FText textValue = FText::Format(LOCTEXT("DeviceHat", "Joystick {0} Axis {1}"), FText::FromString(Device.Name), FText::AsNumber(i));
			EKeys::AddKey(FKeyDetails(key, textValue, FKeyDetails::GamepadKey | FKeyDetails::FloatAxis));
		}
	}

	//Add HATS
	TArray<FString> HatDirections = { "Down", "LeftDown", "Left", "LeftUp", "Up", "RightUp", "Right", "RightDown" };
	TArray<FString> HatAxises = { "X", "Y" };
	for (int i = 0; i < SDL_JoystickNumHats(Device.Joystick); i++) 
	{
		//Buttons for all 8 keys
		for (int j = 0; j < HatDirections.Num(); j++) 
		{
			FString keyName = FString::Printf(TEXT("Joystick %s Hat %i %s"), *Device.Name, i, *HatDirections[j]);
			FKey key{ *keyName };
			Device.Buttons.Add(key);

			//Add a new key if this one isn't there yet.
			if (!EKeys::GetKeyDetails(key).IsValid()) 
			{
				FText textValue = FText::Format(LOCTEXT("DeviceHat", "Joystick {0} Hat {1} {2}"), FText::FromString(Device.Name), FText::AsNumber(i), FText::FromString(HatDirections[j]));
				EKeys::AddKey(FKeyDetails(key, textValue, FKeyDetails::GamepadKey));
			}
		}

		//Two axises.
		for (int k = 0; k < HatAxises.Num(); k++)
		{
			FString keyName = FString::Printf(TEXT("Joystick %s Hat %i %s-Axis"), *Device.Name, i, *HatAxises[k]);
			FKey key{ *keyName };
			Device.Axises.Add(key);

			//Add a new key if this one isn't there yet.
			if (!EKeys::GetKeyDetails(key).IsValid())
			{
				FText textValue = FText::Format(LOCTEXT("DeviceHat", "Joystick {0} Hat {1} {2}-Axis"), FText::FromString(Device.Name), FText::AsNumber(i), FText::FromString(HatAxises[k]));
				EKeys::AddKey(FKeyDetails(key, textValue, FKeyDetails::GamepadKey | FKeyDetails::FloatAxis));
			}
		}
	}

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

int RequenceInputDevice::GetDeviceIndexByWhich(int Which)
{
	for (int i = 0; i < Devices.Num(); i++) {
		if (Devices[i].Which == Which) {
			return i;
		}
	}
	return -1;
}

void RequenceInputDevice::HandleInput_Hat(SDL_Event* e)
{
	FVector2D HatInput = FVector2D::ZeroVector;
	int value = e->jhat.value;

	switch (e->jhat.value)
	{
		case SDL_HAT_CENTERED:
			HatInput = FVector2D::ZeroVector;
			break;
		case SDL_HAT_DOWN:
			HatInput.Y = -1;
			HatInput.X = 0;
			break;
		case SDL_HAT_LEFTDOWN:
			HatInput.Y = -1;
			HatInput.X = -1;
			break;
		case SDL_HAT_LEFT:
			HatInput.Y = 0;
			HatInput.X = -1;
			break;
		case SDL_HAT_LEFTUP:
			HatInput.Y = 1;
			HatInput.X = -1;
			break;
		case SDL_HAT_UP:
			HatInput.Y = 1;
			HatInput.X = 0;
			break;
		case SDL_HAT_RIGHTUP:
			HatInput.Y = 1;
			HatInput.X = 1;
			break;
		case SDL_HAT_RIGHT:
			HatInput.Y = 0;
			HatInput.X = 1;
			break;
		case SDL_HAT_RIGHTDOWN:
			HatInput.Y = -1;
			HatInput.X = 1;
			break;
	}

// 	int index = GetDeviceIndexByWhich(e->jdevice.which);
// 	if (index >= 0) {
// 		Devices[GetDeviceIndexByWhich(e->jdevice.which)].InputState_Hat[e->jhat.hat] = HatInput;
// 	}

	UE_LOG(LogTemp, Log, TEXT("HAT %i input: %s"), e->jhat.hat, *HatInput.ToString());
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

#undef LOCTEXT_NAMESPACE
