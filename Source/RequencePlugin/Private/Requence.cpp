// Fill out your copyright notice in the Description page of Project Settings.

#include "Requence.h"
#include "UObjectIterator.h"
#include "JsonObject.h"
#include "JsonWriter.h"
#include "JsonSerializer.h"
#include "PlatformFilemanager.h"
#include "FileHelper.h"
#include "RequencePlugin.h"

URequence::URequence() 
{
	FRequencePluginModule& RPM = FModuleManager::LoadModuleChecked<FRequencePluginModule>("RequencePlugin");
	if (RPM.InputDevice.IsValid())
	{
		RPM.InputDevice->OnDevicesUpdated.AddUFunction(this, FName("RequenceInputDevicesUpdated"));
	}
}

URequence::~URequence() 
{
	FRequencePluginModule& RPM = FModuleManager::LoadModuleChecked<FRequencePluginModule>("RequencePlugin");
	if (RPM.InputDevice.IsValid())
	{
		RPM.InputDevice->OnDevicesUpdated.RemoveAll(this);
	}
}

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
		ERequenceDeviceType erdt = URequenceDevice::GetDeviceTypeByKeyString(FRIAc.KeyString);
		URequenceDevice* device = GetDeviceByType(erdt);
		if (IsValid(device)) 
		{	//Add to current device
			device->AddAction(FRIAc);
		} 
		else
		{	//Create new device and add key.
			device = CreateDevice(FRIAc.KeyString);
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
		ERequenceDeviceType erdt = URequenceDevice::GetDeviceTypeByKeyString(FRIAx.KeyString);
		URequenceDevice* device = GetDeviceByType(erdt);
		if (IsValid(device))
		{	//Add to current device
			device->AddAxis(FRIAx);
		}
		else
		{	//Create new device and add key.
			device = CreateDevice(FRIAx.KeyString);
			device->AddAxis(FRIAx);
		}
	}

	FillFullAxisActionLists();

	//Add empty bindings so every device has all the mappings. Also sort alphabetically.
	for (URequenceDevice* d : Devices)
	{
		d->AddAllEmpty(FullAxisList, FullActionList);

		//Sort it!
		d->SortAlphabetically();
		d->CompactifyAllKeyNames();
	}

	RequenceInputDevicesUpdated();

	return true;
}

bool URequence::SaveUnrealInput(bool Force)
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

		for (TObjectIterator<UPlayerInput> It; It; ++It)
		{
			It->ForceRebuildingKeyMaps(true);
		}
		return true;
	}

	return false;
}

void URequence::RequenceInputDevicesUpdated()
{
	for (URequenceDevice* URDevice : Devices) {
		URDevice->Connected = false;
	}

	FRequencePluginModule& RPM = FModuleManager::LoadModuleChecked<FRequencePluginModule>("RequencePlugin");
	if (!RPM.InputDevice.IsValid()) { return; }

	for (FSDLDeviceInfo RIDevice : RPM.InputDevice->Devices)
	{
		URequenceDevice* found = nullptr;
		//Try and match the InputDevice to the Stored Device.
		for (URequenceDevice* URDevice : Devices) 
		{
			//match on name
 			if (RIDevice.Name == URDevice->DeviceString) 
			{
				found = URDevice;
 			}
			if (found) break;

			//match on action keys
			for (FRequenceInputAction ac : URDevice->Actions) 
			{
				if (ac.KeyString.Contains(RIDevice.Name))
				{
					found = URDevice;
					break;
				}
			}
			if (found) break;

			//match on axis key.
			for (FRequenceInputAxis ax : URDevice->Axises)
			{
				if (ax.KeyString.Contains(RIDevice.Name))
				{
					found = URDevice;
					break;
				}
			}
			if (found) break;

			//When all else failed.
			URDevice->Connected = false;
		}

		//No device found. Create one!
		if (!found) {
			found = NewObject<URequenceDevice>(this, URequenceDevice::StaticClass());
			found->DeviceType = ERequenceDeviceType::RDT_Unique;
			found->DeviceString = RIDevice.Name;
			found->DeviceName = RIDevice.Name;
			found->RequenceRef = this;
			found->AddAllEmpty(FullAxisList, FullActionList);
			found->SortAlphabetically();
			found->CompactifyAllKeyNames();
			Devices.Add(found);
		}

		//Update status.
		found->DeviceString = RIDevice.Name;
		found->DeviceName = RIDevice.Name;
		found->Connected = true;
	}

	OnUniqueDevicesUpdated.Broadcast();
}

bool URequence::LoadInput(bool ForceDefault)
{
	ClearDevicesAndAxises();

	URequenceSaveObject* RSO_Instance = Cast<URequenceSaveObject>(UGameplayStatics::CreateSaveGameObject(URequenceSaveObject::StaticClass()));
	if (!UGameplayStatics::DoesSaveGameExist(RSO_Instance->SaveSlotName, RSO_Instance->UserIndex) || ForceDefault)
	{
		if (LoadUnrealInput()) { return SaveInput(); }
	} 
	else
	{
		RSO_Instance = Cast<URequenceSaveObject>(UGameplayStatics::LoadGameFromSlot(RSO_Instance->SaveSlotName, RSO_Instance->UserIndex));
		if (RSO_Instance->Devices.Num() > 0)
		{
			FillFullAxisActionLists();
			for (FRequenceSaveObjectDevice SavedDevice : RSO_Instance->Devices)
			{
				URequenceDevice* newDevice = NewObject<URequenceDevice>(this, URequenceDevice::StaticClass());
				newDevice->FromStruct(SavedDevice, this, FullAxisList, FullActionList);
				Devices.Add(newDevice);
			}
			if (Devices.Num() > 0) 
			{ 
				RequenceInputDevicesUpdated();
				return true; 
			}
		}
	}

	return false;
}

bool URequence::SaveInput()
{
	URequenceSaveObject* RSO_Instance = Cast<URequenceSaveObject>(UGameplayStatics::CreateSaveGameObject(URequenceSaveObject::StaticClass()));
	for (URequenceDevice* Device : Devices)
	{
		RSO_Instance->Devices.Add(Device->ToStruct());
	}
	if (RSO_Instance->Devices.Num() > 0)
	{
		return UGameplayStatics::SaveGameToSlot(RSO_Instance, RSO_Instance->SaveSlotName, RSO_Instance->UserIndex);
	}
	return false;
}

bool URequence::ApplyAxisesAndActions(bool Force)
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
		for (TObjectIterator<UPlayerInput> It; It; ++It)
		{
			It->ForceRebuildingKeyMaps(true);
		}
		return true;
	}
	return false;
}

void URequence::OnGameStartup()
{
	if (LoadInput(false)) {
		if (ApplyAxisesAndActions(true)) {
			UE_LOG(LogTemp, Log, TEXT("Requence successfully conducted startup sequence!"));
			return;
		}
		UE_LOG(LogTemp, Log, TEXT("Requence failed to apply axises and actions to runtime on startup!"));
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("Requence failed to load input from sav/UE on startup!"));
	return;
}

void URequence::ExportDeviceAsPreset(URequenceDevice* Device)
{
	//Serialize JSON
	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(Device->GetDeviceAsJson().ToSharedRef(), Writer);

	//Save to file
	FString FileName = FString(Device->DeviceString + "_" + FDateTime::Now().ToString() + ".json");
	bool bAllowOverwriting = true;
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	if (PlatformFile.CreateDirectoryTree(*GetDefaultPresetFilePath()))
	{
		FString AbsolutePath = GetDefaultPresetFilePath() + FileName;

		if (bAllowOverwriting || !PlatformFile.FileExists(*AbsolutePath))
		{
			FFileHelper::SaveStringToFile(OutputString, *AbsolutePath);
			UE_LOG(LogTemp, Log, TEXT("Exported %s as preset to %s"), *Device->DeviceString, *AbsolutePath);
		}
	}
}

bool URequence::ImportDeviceAsPreset(FString AbsolutePath)
{
	//Load in our file
	FString InputString;
	if (FFileHelper::LoadFileToString(InputString, *AbsolutePath))
	{
		UE_LOG(LogTemp, Log, TEXT("Requence is trying to import %s"), *AbsolutePath);

		//Deserialize JSON
		TSharedPtr<FJsonObject> JsonDevice;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(InputString);

		if (FJsonSerializer::Deserialize(Reader, JsonDevice))
		{
			FString sDeviceType;
			if (JsonDevice->TryGetStringField(TEXT("DeviceType"), sDeviceType))
			{
				FillFullAxisActionLists();

				URequenceDevice* NewDevice = NewObject<URequenceDevice>(this, URequenceDevice::StaticClass());
				NewDevice->DeviceType = StringToEnum<ERequenceDeviceType>("ERequenceDeviceType", sDeviceType);
				NewDevice->DeviceName = JsonDevice->GetStringField(TEXT("DeviceName"));
				NewDevice->DeviceString = JsonDevice->GetStringField(TEXT("DeviceString"));
				NewDevice->SetJsonAsActions(JsonDevice->GetArrayField(TEXT("Actions")));
				NewDevice->SetJsonAsAxises(JsonDevice->GetArrayField(TEXT("Axises")));

				NewDevice->RequenceRef = this;
				NewDevice->AddAllEmpty(FullAxisList, FullActionList);
				NewDevice->FilterDeleted(FullAxisList, FullActionList);
				NewDevice->SortAlphabetically();
				NewDevice->Updated = true;

				//Out with the old, in with the new.
				if (GetDeviceByType(NewDevice->DeviceType))
				{
					Devices.Remove(GetDeviceByType(NewDevice->DeviceType));
				}
				Devices.Add(NewDevice);

				UE_LOG(LogTemp, Log, TEXT("Requence imported %s"), *NewDevice->DeviceName);
				return true;
			}
		}
	}
	return false;
}

TArray<FString> URequence::GetImportableDevicePresets()
{
	IFileManager& fm = IFileManager::Get();
	TArray<FString> toReturn;
	fm.FindFiles(toReturn, *GetDefaultPresetFilePath(), TEXT("json"));
	return toReturn;
}

bool URequence::FillFullAxisActionLists()
{
	UInputSettings* Settings = GetMutableDefault<UInputSettings>();
	if (!Settings) { return false; }

	FullActionList.Empty();
	FullAxisList.Empty();

	const TArray<FInputActionKeyMapping>& _Actions = Settings->ActionMappings;
	for (const FInputActionKeyMapping& Each : _Actions)
	{
		FName name = Each.ActionName;
		if (!FullActionList.Contains(name.ToString())) { FullActionList.Add(name.ToString()); }
	}

	const TArray<FInputAxisKeyMapping>& _Axises = Settings->AxisMappings;
	for (const FInputAxisKeyMapping& Each : _Axises)
	{
		FName name = Each.AxisName;
		if (!FullAxisList.Contains(name.ToString())) { FullAxisList.Add(name.ToString()); }
	}

	return true;
}

URequenceDevice* URequence::GetDeviceByString(FString DeviceName)
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
		device->DeviceName = device->DeviceString;
		device->RequenceRef = this;
		Devices.Add(device);
		return device;
	}

	return nullptr;
}

TArray<URequenceDevice*> URequence::GetUniqueDevices()
{
	TArray<URequenceDevice*> toReturn;

	for (URequenceDevice* device : Devices)
	{
		if (device->DeviceType == ERequenceDeviceType::RDT_Unique)
		{
			toReturn.Add(device);
		}
	}

	return toReturn;
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
				UE_LOG(LogTemp, Log, TEXT("- %s: %s. (Ax: %i, Ac: %i)"), *EnumToString<ERequenceDeviceType>("ERequenceDeviceType", Device->DeviceType), *Device->DeviceString, Device->Axises.Num(), Device->Actions.Num());

				if (Device->Axises.Num() > 0)
				{
					for (FRequenceInputAxis ax : Device->Axises)
					{
						UE_LOG(LogTemp, Log, TEXT("  * Axis %s (%s : %f)"), *ax.AxisName, *ax.Key.ToString(), ax.Scale);
					}
				}
				if (Device->Actions.Num() > 0)
				{
					for (FRequenceInputAction ac : Device->Actions)
					{
						UE_LOG(LogTemp, Log, TEXT("  * Action %s (%s)"), *ac.ActionName, *ac.Key.ToString());
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
				UE_LOG(LogTemp, Log, TEXT("  * Axis %s (%s)"), *ax.AxisName, *ax.Key.ToString());
			}
		}
		else { UE_LOG(LogTemp, Log, TEXT("- No Axises found!")); }
		if (Actions.Num() > 0)
		{
			UE_LOG(LogTemp, Log, TEXT("- Actions found: %i"), Actions.Num());
			for (FRequenceInputAction ac : Actions)
			{
				UE_LOG(LogTemp, Log, TEXT("  * Action %s (%s)"), *ac.ActionName, *ac.Key.ToString());
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
	FullAxisList.Empty();
	FullActionList.Empty();
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

bool URequence::GetEditModeEnabled(ERequenceDeviceType& DeviceType)
{
	if (!bEditModeEnabled) 
	{
		EditModeDeviceType = ERequenceDeviceType::RDT_Unknown;
	}
	DeviceType = EditModeDeviceType;
	return bEditModeEnabled;
}

void URequence::SetEditModeStarted(ERequenceDeviceType DeviceType)
{
	bEditModeEnabled = true;
	EditModeDeviceType = DeviceType;
	OnEditModeStarted.Broadcast(DeviceType);
}

void URequence::SetEditModeEnded()
{
	bEditModeEnabled = false;
	EditModeDeviceType = ERequenceDeviceType::RDT_Unknown;
	OnEditModeEnded.Broadcast();
}
