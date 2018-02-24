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

bool URequenceDevice::RebindAction(FString ActionName, FKey NewKey, bool bShift = false, bool bCtrl = false, bool bAlt = false, bool bCmd = false)
{
	return false;
}

bool URequenceDevice::RebindAction(FRequenceInputAction UpdatedAction)
{
	return false;
}

bool URequenceDevice::RebindAxis(FString AxisName, FKey NewKey, float Scale = 1.f)
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
