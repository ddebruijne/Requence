// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RequenceInputDevice.h"

#include "SDL_joystick.h"

/**
 * 
 */
class REQUENCEPLUGIN_API URID_Joystick : public URequenceInputDevice
{
public:
	SDL_Joystick* Joystick = nullptr;

	URID_Joystick();
	
	
};
