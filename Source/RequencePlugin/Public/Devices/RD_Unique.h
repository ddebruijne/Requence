// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RequenceDevice.h"
#include "../RequenceInputDevice.h"
#include "../RequenceStructs.h"
#include "RD_Unique.generated.h"

UCLASS()
class REQUENCEPLUGIN_API URD_Unique : public URequenceDevice
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)	bool bHasPhysicalData = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)	TArray<FString> PhysicalButtons;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)	TArray<FRequencePhysicalAxis> PhysicalAxises;

	//////////////////////////////////////////////////////////////////////////
	// Physical Axis configuration
	//////////////////////////////////////////////////////////////////////////

	void LoadDefaultPhysicalData(FSDLDeviceInfo Data);

	//Retreives a copy of a physical axis struct.
	UFUNCTION(BlueprintCallable)	FRequencePhysicalAxis GetPhysicalAxisByName(FString PhysicalAxisName);

	//Retrieves a copy of a physical axis struct, by its compactified name.
	UFUNCTION(BlueprintCallable)	FRequencePhysicalAxis GetPhysicalAxisByCompactifiedName(FString CompressedAxisName);

	//Updates a physical axis struct. AxisNames must match.
	UFUNCTION(BlueprintCallable)	bool UpdatePhysicalAxisDataPoints(FString AxisName, TArray<FVector2D> DataPoints);

	//Updates a physical axis struct. AxisNames must match.
	UFUNCTION(BlueprintCallable)	bool UpdatePhysicalAxis(FRequencePhysicalAxis toUpdate);


	//////////////////////////////////////////////////////////////////////////
	// JSON Import/Export
	//////////////////////////////////////////////////////////////////////////

	//Retrieves this class' data as a JSON object.
	virtual TSharedPtr<FJsonObject> GetDeviceAsJson() override;

	//Creates a save object device from this device.
	virtual FRequenceSaveObjectDevice ToStruct() override;

	//Fills this device from a requence save object device.
	virtual void FromStruct(FRequenceSaveObjectDevice StructIn, URequence* _RequenceRef,
		TArray<FString> FullAxisList, TArray<FString> FullActionList) override;
};
