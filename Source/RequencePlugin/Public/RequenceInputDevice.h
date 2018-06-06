// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "IInputDevice.h"
#include "InputCoreTypes.h"

#include "SDL.h"
#include "SDL_joystick.h"

DECLARE_MULTICAST_DELEGATE(FRIDUpdate);

struct FHatData
{
	int HatID;
	TMap<uint8, FKey> Buttons;
	TMap<FString, FKey> Axises;

	FHatData() {}
};

struct FSDLDeviceInfo
{
	int Which;
	int InstanceID;
	FString Name;

	SDL_Joystick* Joystick = nullptr;

	TMap<int, float> OldAxisState;	//Map<AxisID, float pos>
	TMap<int, FKey> Axises;			//Map<ButtonID, FKey>

	TMap<int, bool> OldButtonState;	//Map<ButtonID, Bool Down>
	TMap<int, FKey> Buttons;		//Map<ButtonID, FKey>

	TMap<int, uint8> OldHatState;	//Map<HatID, SDL_HAT_STATE>
	TMap<int, FHatData> HatKeys;	//Map<HatID, FHatData>

	FSDLDeviceInfo() {}
};

/*
*  Danny de Bruijne (2018)
*  RequenceInputDevice
*
*  Code to handle SDL joysticks as input devices.
*/
class REQUENCEPLUGIN_API RequenceInputDevice : public IInputDevice
{
public:
	TArray<FString> _HatDirections = {	"Down",	"LeftDown", "Left", "LeftUp", 
										"Up", "RightUp", "Right", "RightDown" };
	TArray<uint8>	_HatDirectionMap = { SDL_HAT_DOWN, SDL_HAT_LEFTDOWN, SDL_HAT_LEFT, SDL_HAT_LEFTUP, 
										 SDL_HAT_UP, SDL_HAT_RIGHTUP, SDL_HAT_RIGHT, SDL_HAT_RIGHTDOWN };
	TArray<FString> _HatAxises = { "X", "Y" };

	FRIDUpdate OnDevicesUpdated;
	bool bOwnsSDL = false;
	TArray<FSDLDeviceInfo> Devices;
	TArray<FRequenceSaveObjectDevice> DeviceProperties;

	RequenceInputDevice() {}
	RequenceInputDevice(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler);
	~RequenceInputDevice();

	void InitSDL();
	static int HandleSDLEvent(void* UserData, SDL_Event* e);
	bool AddDevice(int Which);
	bool RemDevice(int InstanceID);
	int GetDeviceIndexByWhich(int Which);
	void LoadRequenceDeviceProperties();

	void HandleInput_Hat(SDL_Event* e);
	void HandleInput_Button(SDL_Event* e);
	void HandleInput_Axis(SDL_Event* e);

	FVector2D HatStateToVector(uint8 SDL_HAT_STATE);

	//InputDevice Interface
	virtual void Tick(float DeltaTime) override;
	virtual void SendControllerEvents() override;
	virtual void SetMessageHandler(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler) override {}
	virtual bool Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar) override { return false; }
	virtual void SetChannelValue(int32 ControllerId, FForceFeedbackChannelType ChannelType, float Value) override {}
	virtual void SetChannelValues(int32 ControllerId, const FForceFeedbackValues &values) override {}

private:
	TSharedRef<FGenericApplicationMessageHandler> MessageHandler;

};
