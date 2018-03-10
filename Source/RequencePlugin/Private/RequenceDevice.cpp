// Fill out your copyright notice in the Description page of Project Settings.

#include "RequenceDevice.h"
#include "JsonObject.h"

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
		ac.KeyString = CompactifyKeyName(ac.KeyString);
	}
	for (FRequenceInputAxis& ax : Axises)
	{
		ax.KeyString = CompactifyKeyName(ax.KeyString);
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

bool URequenceDevice::StartEditMode()
{
	if (IsValid(RequenceRef)) {
		RequenceRef->SetEditModeStarted(DeviceType);
		return true;
	}
	return false;
}

void URequenceDevice::AddAllEmpty(TArray<FString> FullAxisList, TArray<FString> FullActionList)
{
	for (FString ac : FullActionList)
	{
		if (!HasActionBinding(ac, false))
		{
			FRequenceInputAction newAction;
			newAction.ActionName = ac;
			Actions.Add(newAction);
		}
	}
	for (FString ax : FullAxisList)
	{
		if (!HasAxisBinding(ax, false))
		{
			FRequenceInputAxis newAxis;
			newAxis.AxisName = ax;
			Axises.Add(newAxis);
		}
	}
}

bool URequenceDevice::FilterDeleted(TArray<FString> FullAxisList, TArray<FString> FullActionList)
{
	int deleted = 0;
	if (Actions.Num() > 0)
	{
		for (int i = Actions.Num() - 1; i >= 0; i--)
		{
			if (!FullActionList.Contains(Actions[i].ActionName))
			{
				Actions.RemoveAt(i);
				deleted++;
			}
		}
	}

	if (Axises.Num() > 0)
	{
		for (int i = Axises.Num() - 1; i >= 0; i--)
		{
			if (!FullAxisList.Contains(Axises[i].AxisName))
			{
				Axises.RemoveAt(i);
				deleted++;
			}
		}
	}

	if (deleted > 0) 
	{
		Updated = true;
		return true;
	}
	return false;
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

bool URequenceDevice::RebindAction(FRequenceInputAction OldAction, FRequenceInputAction UpdatedAction)
{
	if (DeviceType == GetDeviceTypeByKeyString(UpdatedAction.Key.ToString()))
	{
		int toChange = -1;
		for (int i = 0; i < Actions.Num(); i++)
		{
			if (Actions[i].ActionName == OldAction.ActionName && Actions[i].Key == OldAction.Key)
			{
				toChange = i;
				break;
			}
		}

		//Outside of for loop since we shouldn't modify a loop while iterating over it. Even though it *should* be safe.
		if (toChange >= 0)
		{
			Actions.RemoveAt(toChange, 1, false);
			FRequenceInputAction a = UpdatedAction;
			a.KeyString = CompactifyKeyName(a.Key.ToString());
			Actions.Insert(a, toChange);
			Updated = true;
			return true;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Requence did not bind a %s key to a %s device!"),
			*EnumToString<ERequenceDeviceType>("ERequenceDeviceType", GetDeviceTypeByKeyString(UpdatedAction.Key.ToString())),
			*EnumToString<ERequenceDeviceType>("ERequenceDeviceType", DeviceType)
		);
	}
	return false;
}

bool URequenceDevice::RebindAxis(FRequenceInputAxis OldAxis, FRequenceInputAxis UpdatedAxis)
{
	if (DeviceType == GetDeviceTypeByKeyString(UpdatedAxis.Key.ToString()))
	{
		int toChange = -1;
		for (int i = 0; i < Axises.Num(); i++)
		{
			if (Axises[i].AxisName == OldAxis.AxisName && Axises[i].Key == OldAxis.Key)
			{
				toChange = i;
				break;
			}
		}

		//Outside of for loop since we shouldn't modify a loop while iterating over it. Even though it *should* be safe.
		if (toChange >= 0)
		{
			Axises.RemoveAt(toChange, 1, false);
			FRequenceInputAxis a = UpdatedAxis;
			a.KeyString = CompactifyKeyName(a.Key.ToString());
			Axises.Insert(a, toChange);
			Updated = true;
			return true;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Requence did not bind a %s key to a %s device!"),
			*EnumToString<ERequenceDeviceType>("ERequenceDeviceType", GetDeviceTypeByKeyString(UpdatedAxis.Key.ToString())),
			*EnumToString<ERequenceDeviceType>("ERequenceDeviceType", DeviceType)
		);
	}
	return false;
}

bool URequenceDevice::DeleteAction(FString ActionName)
{
	int deleted = 0;
	if (Actions.Num() > 0) 
	{
		for (int i = Actions.Num() -1; i >= 0; i--)
		{
			if (Actions[i].ActionName == ActionName)
			{
				Actions.RemoveAt(i);
				deleted++;
				Updated = true;
			}
		}
	}
	if (deleted > 0) { return true; }
	return false;
}

bool URequenceDevice::DeleteAxis(FString AxisName)
{
	int deleted = 0;
	if (Axises.Num() > 0)
	{
		for (int i = Axises.Num() -1; i >= 0; i--)
		{
			if (Axises[i].AxisName == AxisName)
			{
				Axises.RemoveAt(i);
				deleted++;
				Updated = true;
			}
		}
	}
	if (deleted > 0) { return true; }
	return false;
}

void URequenceDevice::FromStruct(FRequenceSaveObjectDevice StructIn, URequence* _RequenceRef, 
	TArray<FString> FullAxisList, TArray<FString> FullActionList)
{
	DeviceString = StructIn.DeviceString;
	DeviceName = StructIn.DeviceName;
	DeviceType = StructIn.DeviceType;
	Actions = StructIn.Actions;
	Axises = StructIn.Axises;
	RequenceRef = _RequenceRef;

	AddAllEmpty(FullAxisList, FullActionList);
	FilterDeleted(FullAxisList, FullActionList);
	SortAlphabetically();
}

FRequenceSaveObjectDevice URequenceDevice::ToStruct()
{
	FRequenceSaveObjectDevice toReturn;
	toReturn.DeviceString = DeviceString;
	toReturn.DeviceName = DeviceName;
	toReturn.DeviceType = DeviceType;
	toReturn.Actions = Actions;
	toReturn.Axises = Axises;

	//Filter empty keys
	if (toReturn.Actions.Num() > 0)
	{
		for (int i = toReturn.Actions.Num() - 1; i >= 0; i--)
		{
			if (toReturn.Actions[i].Key == FKey())
			{
				toReturn.Actions.RemoveAt(i);
			}
		}
	}

	//Filter empty keys
	if (toReturn.Axises.Num() > 0)
	{
		for (int i = toReturn.Axises.Num() - 1; i >= 0; i--)
		{
			if (toReturn.Axises[i].Key == FKey())
			{
				toReturn.Axises.RemoveAt(i);
			}
		}
	}

	return toReturn;
}

TSharedPtr<FJsonObject> URequenceDevice::GetDeviceAsJson()
{
	TSharedPtr<FJsonObject> Preset = MakeShareable(new FJsonObject);
	Preset->SetStringField("DeviceString", DeviceString);
	Preset->SetStringField("DeviceName", DeviceName);
	Preset->SetStringField("DeviceType", EnumToString<ERequenceDeviceType>("ERequenceDeviceType", DeviceType));

	Preset->SetArrayField("Actions", GetActionsAsJson());
	Preset->SetArrayField("Axises", GetAxisesAsJson());
	Preset->SetStringField("Timestamp", FDateTime::Now().ToString());

	return Preset;
}

TArray<TSharedPtr<FJsonValue>> URequenceDevice::GetActionsAsJson()
{
	TArray<TSharedPtr<FJsonValue>> JsonActions;
	for (FRequenceInputAction ac : Actions)
	{
		if (ac.Key == FKey()) { continue; }	//Skip if empty.

		TSharedPtr<FJsonObject> Action = MakeShareable(new FJsonObject);
		Action->SetStringField("ActionName", ac.ActionName);
		Action->SetStringField("Key", ac.Key.ToString());
		Action->SetBoolField("bShift", ac.bShift);
		Action->SetBoolField("bCtrl", ac.bCtrl);
		Action->SetBoolField("bAlt", ac.bAlt);
		Action->SetBoolField("bCmd", ac.bCmd);

		TSharedRef<FJsonValueObject> ActionValue = MakeShareable(new FJsonValueObject(Action));
		JsonActions.Add(ActionValue);
	}
	return JsonActions;
}

TArray<TSharedPtr<FJsonValue>> URequenceDevice::GetAxisesAsJson()
{
	TArray<TSharedPtr<FJsonValue>> JsonAxises;
	for (FRequenceInputAxis ax : Axises)
	{
		if (ax.Key == FKey()) { continue; }	//Skip if empty.

		TSharedPtr<FJsonObject> Axis = MakeShareable(new FJsonObject);
		Axis->SetStringField("AxisName", ax.AxisName);
		Axis->SetStringField("Key", ax.Key.ToString());
		Axis->SetNumberField("Scale", (double)ax.Scale);

		TSharedRef<FJsonValueObject> AxisValue = MakeShareable(new FJsonValueObject(Axis));
		JsonAxises.Add(AxisValue);
	}
	return JsonAxises;
}

void URequenceDevice::SetJsonAsActions(TArray<TSharedPtr<FJsonValue>> _Actions)
{
	for (int i = 0; i < _Actions.Num(); i++)
	{
		TSharedPtr<FJsonObject> JsonAction = _Actions[i]->AsObject();

		if (JsonAction->GetStringField(TEXT("Key")) != "None")
		{
			FRequenceInputAction NewAction;
			NewAction.ActionName = JsonAction->GetStringField(TEXT("ActionName"));
			NewAction.Key = FKey(FName(*JsonAction->GetStringField(TEXT("Key"))));
			NewAction.KeyString = CompactifyKeyName(NewAction.Key.ToString());
			NewAction.bShift = JsonAction->GetBoolField(TEXT("bShift"));
			NewAction.bCtrl = JsonAction->GetBoolField(TEXT("bCtrl"));
			NewAction.bAlt = JsonAction->GetBoolField(TEXT("bAlt"));
			NewAction.bCmd = JsonAction->GetBoolField(TEXT("bCmd"));
			Actions.Add(NewAction);
		}
	}
}

void URequenceDevice::SetJsonAsAxises(TArray<TSharedPtr<FJsonValue>> _Axises)
{
	for (int i = 0; i < _Axises.Num(); i++)
	{
		TSharedPtr<FJsonObject> JsonAxis = _Axises[i]->AsObject();

		if (JsonAxis->GetStringField(TEXT("Key")) != "None")
		{
			FRequenceInputAxis NewAxis;
			NewAxis.AxisName = JsonAxis->GetStringField(TEXT("AxisName"));
			NewAxis.Key = FKey(FName(*JsonAxis->GetStringField(TEXT("Key"))));
			NewAxis.KeyString = CompactifyKeyName(NewAxis.Key.ToString());
			NewAxis.Scale = JsonAxis->GetNumberField(TEXT("Scale"));
			Axises.Add(NewAxis);
		}
	}
}
