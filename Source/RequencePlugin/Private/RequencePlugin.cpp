// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "RequencePlugin.h"
#include "IPluginManager.h"
#include "Engine.h"
#include "Core.h"
#include "ModuleManager.h"
#include "Internationalization.h"
#include "SharedPointer.h"
#include "UObjectGlobals.h"

#define LOCTEXT_NAMESPACE "FRequencePluginModule"

void FRequencePluginModule::StartupModule()
{
	FString BaseDir = IPluginManager::Get().FindPlugin("RequencePlugin")->GetBaseDir();
	FString LibraryPath;

#if PLATFORM_WINDOWS
#if PLATFORM_32BITS
	LibraryPath = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/Win32/SDL2.dll"));
	UE_LOG(LogTemp, Log, TEXT("Loading 32-bit SDL at %s"), *LibraryPath);
#else
	LibraryPath = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/Win64/SDL2.dll"));
	UE_LOG(LogTemp, Log, TEXT("Loading 64-bit SDL at %s"), *LibraryPath);
#endif
#endif

	SDLLibrary = !LibraryPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*LibraryPath) : nullptr;

	if (!SDLLibrary)
	{
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Requence", "Failed to load SDL. Is the DLL available for your OS/Architecture?"));
	} 

	UE_LOG(LogTemp, Log, TEXT("RequencePlugin Started up"));

	SDLManager = new URequenceSDL();
	SDLManager->Initialize();
}

void FRequencePluginModule::ShutdownModule()
{
	FPlatformProcess::FreeDllHandle(SDLLibrary);
	SDLLibrary = nullptr;

	delete(SDLManager);
	SDLManager = nullptr;

	UE_LOG(LogTemp, Log, TEXT("RequencePlugin Shut down"));
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRequencePluginModule, RequencePlugin)