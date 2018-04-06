// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IRequencePlugin.h"
#include "RequenceInputDevice.h"

class FRequencePluginModule : public IRequencePlugin
{
private:
	void* SDLLibrary;

public:
	virtual TSharedPtr<class IInputDevice> CreateInputDevice(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler) override;

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

};