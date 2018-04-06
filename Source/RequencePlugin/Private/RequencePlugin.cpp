// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "RequencePlugin.h"
#include "IPluginManager.h"
#include "Engine.h"
#include "Core.h"
#include "ModuleManager.h"
#include "Internationalization.h"
#include "SharedPointer.h"
#include "UObjectGlobals.h"

#define LOCTEXT_NAMESPACE "RequencePlugin"

DEFINE_LOG_CATEGORY_STATIC(LogRequence, Log, All);

TSharedPtr<class IInputDevice> FRequencePluginModule::CreateInputDevice(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler)
{
	UE_LOG(LogRequence, Log, TEXT("Created new Input Device"));
	return MakeShareable(new RequenceInputDevice(InMessageHandler));
}

void FRequencePluginModule::StartupModule()
{
	IRequencePlugin::StartupModule();

	FString BaseDir = IPluginManager::Get().FindPlugin("RequencePlugin")->GetBaseDir();
	FString LibraryPath;

#if PLATFORM_WINDOWS
#if PLATFORM_32BITS
	LibraryPath = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/Win32/SDL2.dll"));
	UE_LOG(LogRequence, Log, TEXT("Loading 32-bit SDL at %s"), *LibraryPath);
#else
	LibraryPath = FPaths::Combine(*BaseDir, TEXT("Binaries/ThirdParty/Win64/SDL2.dll"));
	UE_LOG(LogRequence, Log, TEXT("Loading 64-bit SDL at %s"), *LibraryPath);
#endif
#endif

	SDLLibrary = !LibraryPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*LibraryPath) : nullptr;

	if (!SDLLibrary)
	{
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT("Requence", "Failed to load SDL. Is the DLL available for your OS/Architecture?"));
	} 

	IModularFeatures::Get().RegisterModularFeature(IInputDeviceModule::GetModularFeatureName(), this);
	//InputDevice = MakeShareable(new RequenceInputDevice());

	UE_LOG(LogRequence, Log, TEXT("RequencePlugin Started up"));
}

void FRequencePluginModule::ShutdownModule()
{
	FPlatformProcess::FreeDllHandle(SDLLibrary);
	SDLLibrary = nullptr;

	IModularFeatures::Get().UnregisterModularFeature(IInputDeviceModule::GetModularFeatureName(), this);

	IRequencePlugin::ShutdownModule();

	UE_LOG(LogRequence, Log, TEXT("RequencePlugin Shut down"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FRequencePluginModule, RequencePlugin)

