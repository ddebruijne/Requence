// Fill out your copyright notice in the Description page of Project Settings.

#include "RequenceDevice.h"

URequenceDevice::URequenceDevice()
{
}

ERequenceDeviceType URequenceDevice::GetDeviceTypeByKeyString(FString KeyString)
{
	if (KeyString.Contains("Mouse")) { return ERequenceDeviceType::RDT_Mouse; }
	if (KeyString.Contains("Gamepad")) { return ERequenceDeviceType::RDT_Gamepad; }
	if (KeyString.Contains("Joystick")) { return ERequenceDeviceType::RDT_Unique; }
	if (KeyString.Contains("GenericUSBController")) { return ERequenceDeviceType::RDT_Unique; }
	if (KeyString.Contains("MotionController")) { return ERequenceDeviceType::RDT_MotionController; }
	if (KeyString.Contains("Oculus")) { return ERequenceDeviceType::RDT_MotionController; }
	if (!KeyString.IsEmpty()) { return ERequenceDeviceType::RDT_Keyboard; }
	return ERequenceDeviceType::RDT_Unknown;
}

FString URequenceDevice::GetDeviceNameByType(ERequenceDeviceType DeviceType)
{
	switch (DeviceType)
	{
	case ERequenceDeviceType::RDT_Unknown:
		return "Unknown";
		break;
	case ERequenceDeviceType::RDT_Keyboard:
		return "Keyboard";
		break;
	case ERequenceDeviceType::RDT_Mouse:
		return "Mouse";
		break;
	case ERequenceDeviceType::RDT_Gamepad:
		return "Gamepad";
		break;
	case ERequenceDeviceType::RDT_MotionController:
		return "Motion Controller";
		break;
	case ERequenceDeviceType::RDT_Unique:
		return "Unknown";
		break;
	default:
		return "Unknown";
		break;
	}
}

void URequenceDevice::CompactifyAllKeyNames()
{
	for (FRequenceInputAction& ac : Actions)
	{
		ac.KeyAsString = CompactifyKeyName(ac.KeyAsString);
	}
	for (FRequenceInputAxis& ax : Axises)
	{
		ax.KeyAsString = CompactifyKeyName(ax.KeyAsString);
	}
}

FString URequenceDevice::CompactifyKeyName(FString InName)
{
	FString outName = InName;
	outName = outName.Replace(TEXT("Gamepad "), TEXT(""));
	outName = outName.Replace(TEXT("MotionController "), TEXT(""));
	outName = outName.Replace(TEXT("Mouse "), TEXT(""));
	outName = outName.Replace(TEXT("Joystick"), TEXT(""));
	outName = outName.Replace(TEXT("stick"), TEXT("stk"));
	outName = outName.Replace(TEXT("button"), TEXT("btn"));
	outName = outName.Replace(TEXT("_"), TEXT(" "));
	outName.Trim();
	return outName;
}

bool URequenceDevice::HasActionBinding(FString ActionName, bool MustBeBound)
{
	for (FRequenceInputAction ac : Actions)
	{
		if (ac.ActionName == ActionName) 
		{ 
			if (MustBeBound)
			{
				if (ac.Key != FKey()) { return true; }
			}
			else { return true; }
		}
	}
	return false;
}

bool URequenceDevice::HasAxisBinding(FString AxisName, bool MustBeBound)
{
	for (FRequenceInputAxis ax : Axises)
	{
		if (ax.AxisName == AxisName) 
		{ 
			if (MustBeBound) {
				if (ax.Key != FKey()) { return true; }
			}
			else { return true; }
		}
	}
	return false;
}

void URequenceDevice::SortAlphabetically()
{
	//Actions
	bool acswapped = true;
	int acJ = 0;
	while (acswapped)
	{
		acswapped = false;
		acJ++;

		for (int i = 0; i < (Actions.Num() - acJ); i++)
		{
			if (Actions[i].ActionName > Actions[i + 1].ActionName)
			{
				FRequenceInputAction cache = Actions[i];
				Actions[i] = Actions[i + 1];
				Actions[i + 1] = cache;
				acswapped = true;
			}
		}
	}

	//Axises
	bool axswapped = true;
	int axJ = 0;
	while (axswapped)
	{
		axswapped = false;
		axJ++;

		for (int i = 0; i < (Axises.Num() - axJ); i++)
		{
			if (Axises[i].AxisName > Axises[i + 1].AxisName)
			{
				FRequenceInputAxis cache = Axises[i];
				Axises[i] = Axises[i + 1];
				Axises[i + 1] = cache;
				axswapped = true;
			}
		}
	}
}

bool URequenceDevice::AddAction(FRequenceInputAction _action)
{
	//Check for duplicates.
	for (FRequenceInputAction action : Actions)
	{
		if (_action.Key == action.Key && _action.ActionName == action.ActionName)
		{
			return false;
		}
	}

	Actions.Add(_action);
	Updated = true;
	return true;
}

bool URequenceDevice::AddAxis(FRequenceInputAxis _axis)
{
	//Check for duplicates.
	for (FRequenceInputAxis axis : Axises)
	{
		if (_axis.Key == axis.Key && _axis.AxisName == axis.AxisName) {
			return false;
		}
	}

	Axises.Add(_axis);
	Updated = true;
	return true;
}

bool URequenceDevice::RebindAction(FRequenceInputAction UpdatedAction)
{
	return false;
}

bool URequenceDevice::RebindAxis(FRequenceInputAxis UpdatedAxis)
{
	return false;
}

bool URequenceDevice::DeleteAction(FString ActionName)
{
	if (Actions.Num() > 0) 
	{
		for (int i = Actions.Num() -1; i >= 0; i--)
		{
			if (Actions[i].ActionName == ActionName)
			{
				Actions.RemoveAt(i);
				Updated = true;
				return true;
			}
		}
	}
	return false;
}

bool URequenceDevice::DeleteAxis(FString AxisName)
{
	if (Axises.Num() > 0)
	{
		for (int i = Axises.Num() -1; i >= 0; i--)
		{
			if (Axises[i].AxisName == AxisName)
			{
				Axises.RemoveAt(i);
				Updated = true;
				return true;
			}
		}
	}
	return false;
}
