// Fill out your copyright notice in the Description page of Project Settings.

#include "Requence.h"
#include "UObjectIterator.h"

URequence::URequence() {}

bool URequence::LoadUnrealInput()
{
	ClearDevicesAndAxises();

	UInputSettings* Settings = GetMutableDefault<UInputSettings>();
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

	//Add empty bindings so every device has all the mappings. Also sort alphabetically.
	for (URequenceDevice* d : Devices)
	{
		for (FRequenceInputAction ac : Actions)
		{
			if (!d->HasActionBinding(ac.ActionName)) 
			{ 
				FRequenceInputAction newAction;
				newAction.ActionName = ac.ActionName;
				d->Actions.Add(newAction);
			}
		}
		for (FRequenceInputAxis ax : Axises)
		{
			if (!d->HasAxisBinding(ax.AxisName)) 
			{ 
				FRequenceInputAxis newAxis;
				newAxis.AxisName = ax.AxisName;
				d->Axises.Add(newAxis);
			}
		}

		//Sort it!
		d->SortAlphabetically();
	}

	//Add Axis Config

	//Separate unique device into separate devices.

	return true;
}

bool URequence::SaveUnrealInput(bool Force = false)
{
	UInputSettings* Settings = GetMutableDefault<UInputSettings>();
	if (!Settings) { return false; }

	if (HasUpdated() || Force)
	{
		//Todo: Store a backup of these mappings - only empty them after the fact when its safe.
		Settings->ActionMappings.Empty();
		Settings->AxisMappings.Empty();

		for (URequenceDevice* d : Devices)
		{
			for (FRequenceInputAction ac : d->Actions)
			{
				if (ac.Key != FKey())
				{
					FInputActionKeyMapping NewAction;
					NewAction.ActionName = FName(*ac.ActionName);
					NewAction.Key = ac.Key;
					NewAction.bShift = ac.bShift;
					NewAction.bCtrl = ac.bCmd;
					NewAction.bAlt = ac.bAlt;
					NewAction.bCmd = ac.bCmd;
					Settings->ActionMappings.Add(NewAction);
				}
			}

			for (FRequenceInputAxis ax : d->Axises)
			{
				if (ax.Key != FKey())
				{
					FInputAxisKeyMapping NewAxis;
					NewAxis.AxisName = FName(*ax.AxisName);
					NewAxis.Key = ax.Key;
					NewAxis.Scale = ax.Scale;
					Settings->AxisMappings.Add(NewAxis);
				}
			}
		}

		Settings->SaveKeyMappings();
		return true;
	}

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

void URequence::DebugPrint(bool UseDevices = true)
{
	UE_LOG(LogTemp, Log, TEXT("Requence Debug Print:"));
	if (UseDevices)
	{
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
	else
	{
		if (Axises.Num() > 0)
		{
			UE_LOG(LogTemp, Log, TEXT("- Axises found: %i"), Axises.Num());
			for (FRequenceInputAxis ax : Axises)
			{
				UE_LOG(LogTemp, Log, TEXT("  * Axis %s (%s)"), *ax.AxisName, *ax.KeyAsString);
			}
		}
		else { UE_LOG(LogTemp, Log, TEXT("- No Axises found!")); }
		if (Actions.Num() > 0)
		{
			UE_LOG(LogTemp, Log, TEXT("- Actions found: %i"), Actions.Num());
			for (FRequenceInputAction ac : Actions)
			{
				UE_LOG(LogTemp, Log, TEXT("  * Action %s (%s)"), *ac.ActionName, *ac.KeyAsString);
			}
		}
		else { UE_LOG(LogTemp, Log, TEXT("- No Actions found!")); }
	}
}

void URequence::ClearDevicesAndAxises()
{
	Actions.Empty();
	Axises.Empty();
	Devices.Empty();
}

bool URequence::HasUpdated()
{
	if (Devices.Num() > 0)
	{
		for (URequenceDevice* d : Devices)
		{
			if (d->Updated) { return true; }
		}
	}
	return false;
}
