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
	for (FRequenceInputAction action : Actions)
	{
		if (_action.Key == action.Key && _action.ActionName == action.ActionName)
		{
			return false;
		}
	}

	Actions.Add(_action);
	return true;
}

bool URequenceDevice::AddAxis(FRequenceInputAxis _axis)
{
	for (FRequenceInputAxis axis : Axises)
	{
		if (_axis.Key == axis.Key && _axis.Key == axis.Key) {
			return false;
		}
	}

	Axises.Add(_axis);
	return true;
}

bool URequenceDevice::RebindAction(FString ActionName, FKey NewKey, bool bShift = false, bool bCtrl = false, bool bAlt = false, bool bCmd = false)
{

}

bool URequenceDevice::RebindAxis(FString AxisName, FKey NewKey, float Scale = 1.f)
{

}

bool URequenceDevice::DeleteAction(FString ActionName)
{

}

bool URequenceDevice::DeleteAxis(FString ActionName)
{

}
