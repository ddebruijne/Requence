// Fill out your copyright notice in the Description page of Project Settings.

#include "Requence.h"

URequence::URequence() {}

bool URequence::LoadUnrealInput()
{
	ClearDevicesAndAxises();

	const UInputSettings* Settings = GetDefault<UInputSettings>();
	if (!Settings) { return false; }

	//ACTIONS
	const TArray<FInputActionKeyMapping>& _Actions = Settings->ActionMappings;
	for (const FInputActionKeyMapping& Each : _Actions)
	{
		FRequenceInputAction FRIAc = FRequenceInputAction(Each);
		Actions.Add(FRIAc);

 		//Grab device
		ERequenceDeviceType erdt = URequenceDevice::GetDeviceTypeByKeyString(FRIAc.KeyAsString);
		URequenceDevice* device = GetDeviceByType(erdt);
		if (IsValid(device)) 
		{	//Add to current device
			device->AddAction(FRIAc);
		} 
		else
		{	//Create new device and add key.
			device = CreateDevice(FRIAc.KeyAsString);
			device->AddAction(FRIAc);
		}
	}

	//AXISES
	const TArray<FInputAxisKeyMapping>& _Axises = Settings->AxisMappings;
	for (const FInputAxisKeyMapping& Each : _Axises)
	{
		FRequenceInputAxis FRIAx = FRequenceInputAxis(Each);
		Axises.Add(FRIAx);

		//Grab device
		ERequenceDeviceType erdt = URequenceDevice::GetDeviceTypeByKeyString(FRIAx.KeyAsString);
		URequenceDevice* device = GetDeviceByType(erdt);
		if (IsValid(device))
		{	//Add to current device
			device->AddAxis(FRIAx);
		}
		else
		{	//Create new device and add key.
			device = CreateDevice(FRIAx.KeyAsString);
			device->AddAxis(FRIAx);
		}
	}

	//Add Axis Config

	//Separate unique device into separate devices.

	return true;
}

bool URequence::SaveUnrealInput()
{
	return false;
}

URequenceDevice* URequence::GetDeviceByName(FString DeviceName)
{
	if (Devices.Num() > 0)
	{
		for (URequenceDevice* Device : Devices)
		{
			if (Device->DeviceString == DeviceName) 
			{
				return Device;
			}
		}
	}

	return nullptr;
}

URequenceDevice* URequence::GetDeviceByType(ERequenceDeviceType DeviceType)
{
	if (Devices.Num() > 0)
	{
		for (URequenceDevice* Device : Devices)
		{
			if (Device->DeviceType == DeviceType)
			{
				return Device;
			}
		}
	}

	return nullptr;
}

URequenceDevice* URequence::CreateDevice(FString KeyName)
{
	ERequenceDeviceType NewDeviceType = URequenceDevice::GetDeviceTypeByKeyString(KeyName);

	if (!IsValid(GetDeviceByType(NewDeviceType)))
	{
		URequenceDevice* device = NewObject<URequenceDevice>(this, URequenceDevice::StaticClass());
		device->DeviceType = NewDeviceType;
		device->DeviceString = URequenceDevice::GetDeviceNameByType(NewDeviceType);
		Devices.Add(device);
		return device;
	}

	return nullptr;
}

void URequence::DebugPrint()
{
	UE_LOG(LogTemp, Log, TEXT("Requence Debug Print:"));
	if (Devices.Num() > 0) 
	{
		UE_LOG(LogTemp, Log, TEXT("- Devices found: %i"), Devices.Num());
		UE_LOG(LogTemp, Log, TEXT("- Actions found: %i"), Actions.Num());
		UE_LOG(LogTemp, Log, TEXT("- Axises found: %i"), Axises.Num());
		for (URequenceDevice* Device : Devices)
		{
			UE_LOG(LogTemp, Log, TEXT("- %s: %s. (Ax: %i, Ac: %i)"), *EnumToString(TEXT("ERequenceDeviceType"), Device->DeviceType), *Device->DeviceString, Device->Axises.Num(), Device->Actions.Num());

			if (Axises.Num() > 0)
			{
				for (FRequenceInputAxis ax : Device->Axises)
				{
					UE_LOG(LogTemp, Log, TEXT("  * Axis %s (%s)"), *ax.AxisName, *ax.KeyAsString);
				}
			}
			if (Actions.Num() > 0)
			{
				for (FRequenceInputAction ac : Device->Actions)
				{
					UE_LOG(LogTemp, Log, TEXT("  * Action %s (%s)"), *ac.ActionName, *ac.KeyAsString);
				}
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("- No devices found!"));
	}
}

void URequence::ClearDevicesAndAxises()
{
	Actions.Empty();
	Axises.Empty();
	Devices.Empty();
}
