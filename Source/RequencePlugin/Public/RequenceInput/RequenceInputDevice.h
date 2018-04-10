// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "IInputDevice.h"
#include "InputCoreTypes.h"

#include "SDL.h"
#include "SDL_joystick.h"

struct FSDLDeviceInfo
{
	int Which;
	int InstanceID;
	FString Name;

	SDL_Joystick* Joystick = nullptr;

	TArray<FKey> Axises;
	TArray<FKey> Buttons;

//	TMap<int, FVector2D>	InputState_Hat;
// 	TArray<bool>	InputState_Buttons;
// 	TArray<float>	InputState_Axes;

	FSDLDeviceInfo() {}
};

/**
 * 
 */
class REQUENCEPLUGIN_API RequenceInputDevice : public IInputDevice
{
public:

	bool bOwnsSDL = false;
	TArray<FSDLDeviceInfo> Devices;

	RequenceInputDevice() {}
	RequenceInputDevice(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler);
	~RequenceInputDevice();

	void InitSDL();
	static int HandleSDLEvent(void* UserData, SDL_Event* e);
	bool AddDevice(int Which);
	bool RemDevice(int InstanceID);
	int GetDeviceIndexByWhich(int Which);

	void HandleInput_Hat(SDL_Event* e);

	//InputDevice Interface
	virtual void Tick(float DeltaTime) override;
	virtual void SendControllerEvents() override;
	virtual void SetMessageHandler(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler) override;
	virtual bool Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar) override;
	virtual void SetChannelValue(int32 ControllerId, FForceFeedbackChannelType ChannelType, float Value) override;
	virtual void SetChannelValues(int32 ControllerId, const FForceFeedbackValues &values) override;

private:
	TSharedRef<FGenericApplicationMessageHandler> MessageHandler;

};
