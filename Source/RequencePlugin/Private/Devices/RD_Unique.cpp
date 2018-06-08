#include "RD_Unique.h"

void URD_Unique::LoadDefaultPhysicalData(FSDLDeviceInfo Data)
{
	if (bHasPhysicalData) return;

	PhysicalAxises.Empty();
	PhysicalButtons.Empty();

	for (auto& Axises : Data.Axises)
	{
		PhysicalAxises.Add(FRequencePhysicalAxis(Axises.Value.ToString()));
	}
	for (auto& Buttons : Data.Buttons)
	{
		PhysicalButtons.Add(Buttons.Value.ToString());
	}

	//todo: add hats

	bHasPhysicalData = true;
}

FRequencePhysicalAxis URD_Unique::GetPhysicalAxisByName(FString PhysicalAxisName)
{
	for (FRequencePhysicalAxis pa : PhysicalAxises) 
	{
		if (pa.Axis == PhysicalAxisName) { return pa; }
	}

	return FRequencePhysicalAxis();
}

bool URD_Unique::UpdatePhysicalAxisDataPoints(FString AxisName, TArray<FVector2D> DataPoints)
{
	for (int i = 0; i < PhysicalAxises.Num(); i++) 
	{
		if (PhysicalAxises[i].Axis == AxisName) {
			PhysicalAxises[i].DataPoints = DataPoints;
			this->Updated = true;
			return true;
		}
	}

	return false;
}

bool URD_Unique::UpdatePhysicalAxis(FRequencePhysicalAxis toUpdate)
{
	for (int i = 0; i < PhysicalAxises.Num(); i++)
	{
		if (PhysicalAxises[i].Axis == toUpdate.Axis) {
			PhysicalAxises[i] = toUpdate;
			this->Updated = true;
			return true;
		}
	}
	return false;
}

TSharedPtr<FJsonObject> URD_Unique::GetDeviceAsJson()
{
	TSharedPtr<FJsonObject> Preset = URequenceDevice::GetDeviceAsJson();

	TArray<TSharedPtr<FJsonValue>> axises;
	for (FRequencePhysicalAxis pa : PhysicalAxises)
	{
		TSharedPtr<FJsonObject> JSONPhysicalAxis = MakeShareable(new FJsonObject);
		JSONPhysicalAxis->SetStringField("Axis", pa.Axis);

		TArray<TSharedPtr<FJsonValue>> datapoints;
		for (FVector2D dp : pa.DataPoints) {
			TSharedPtr<FJsonObject> datapoint = MakeShareable(new FJsonObject);
			datapoint->SetNumberField("X", dp.X);
			datapoint->SetNumberField("Y", dp.Y);

			TSharedRef<FJsonValueObject> datapointvalue = MakeShareable(new FJsonValueObject(datapoint));
			datapoints.Add(datapointvalue);
		}
		JSONPhysicalAxis->SetArrayField("CurveDataPoints", datapoints);
		JSONPhysicalAxis->SetStringField("InputRange", EnumToString<ERequencePAInputRange>("ERequencePAInputRange", pa.InputRange));

		TSharedRef<FJsonValueObject> PhysicalAxisValue = MakeShareable(new FJsonValueObject(JSONPhysicalAxis));
		axises.Add(PhysicalAxisValue);
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
