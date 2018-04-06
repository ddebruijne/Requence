// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ModuleManager.h"
#include "RequenceSDL.h"

class FRequencePluginModule : public IModuleInterface
{
private:
	void* SDLLibrary;

public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	URequenceSDL* SDLManager;
};