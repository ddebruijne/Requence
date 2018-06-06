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
	
	void LoadDefaultPhysicalData(FSDLDeviceInfo Data);

	UFUNCTION(BlueprintCallable)
	FRequencePhysicalAxis GetPhysicalAxisByName(FString PhysicalAxisName);

	UFUNCTION(BlueprintCallable)
	bool UpdatePhysicalAxisDataPoints(FString AxisName, TArray<FVector2D> DataPoints);

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
