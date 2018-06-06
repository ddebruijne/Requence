#include "RD_Unique.h"

void URD_Unique::LoadDefaultPhysicalData(FSDLDeviceInfo Data)
{
	if (bHasPhysicalData) return;

	PhysicalAxises.Empty();
	PhysicalButtons.Empty();

	for (auto& Axises : Data.Axises)
	{
		PhysicalAxises.Add(Axises.Value.ToString());
	}
	for (auto& Buttons : Data.Buttons)
	{
		PhysicalButtons.Add(Buttons.Value.ToString());
	}

	//todo: add hats

	bHasPhysicalData = true;
}

TSharedPtr<FJsonObject> URD_Unique::GetDeviceAsJson()
{
	TSharedPtr<FJsonObject> Preset = URequenceDevice::GetDeviceAsJson();

	TArray<TSharedPtr<FJsonValue>> axises;
	for (FString str : PhysicalAxises)
	{
		TSharedPtr<FJsonValueString> val = MakeShareable(new FJsonValueString(str));
		axises.Add(val);
	}
	Preset->SetArrayField("PhysicalAxises", axises);

	return Preset;
}

FRequenceSaveObjectDevice URD_Unique::ToStruct()
{
	FRequenceSaveObjectDevice toReturn = URequenceDevice::ToStruct();
	toReturn.PhysicalAxises = PhysicalAxises;
	toReturn.PhysicalButtons = PhysicalButtons;
	return toReturn;
}

void URD_Unique::FromStruct(FRequenceSaveObjectDevice StructIn, URequence* _RequenceRef, TArray<FString> FullAxisList, TArray<FString> FullActionList)
{
	URequenceDevice::FromStruct(StructIn, _RequenceRef, FullAxisList, FullActionList);
	PhysicalAxises = StructIn.PhysicalAxises;
	PhysicalButtons = StructIn.PhysicalButtons;
	bHasPhysicalData = true;
}
