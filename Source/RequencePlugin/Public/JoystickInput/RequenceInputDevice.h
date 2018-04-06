// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IInputDevice.h"

/**
 * 
 */
class REQUENCEPLUGIN_API URequenceInputDevice : public IInputDevice
{
public:
	int DeviceIndex = -1;
	int InstanceID = -1;
	FString Name = "Unknown";

	URequenceInputDevice();
};
