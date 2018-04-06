// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IInputDevice.h"

#include "SDL.h"
#include "SDL_joystick.h"

/**
 * 
 */
class REQUENCEPLUGIN_API RequenceInputDevice : public IInputDevice
{
public:
	bool bOwnsSDL = false;

	RequenceInputDevice() {}
	RequenceInputDevice(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler);
	~RequenceInputDevice();

	void InitSDL();
	static int HandleSDLEvent(void* UserData, SDL_Event* e);

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
