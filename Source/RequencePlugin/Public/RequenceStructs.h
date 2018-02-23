#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerInput.h"
#include "RequenceStructs.generated.h"

template<typename T>
static FString EnumToString(const FString& enumName, const T value)
{
	UEnum* pEnum = FindObject<UEnum>(ANY_PACKAGE, *enumName);
	return *(pEnum ? pEnum->GetNameStringByIndex(static_cast<uint8>(value)) : "null");
}

//Device Type
UENUM(BlueprintType)
enum class ERequenceDeviceType : uint8
{
	RDT_Unknown				UMETA(DisplayName = "Unknown"),
	RDT_Keyboard			UMETA(DisplayName = "Keyboard"),
	RDT_Mouse				UMETA(DisplayName = "Mouse"),
	RDT_Gamepad				UMETA(DisplayName = "Gamepad"),
	RDT_MotionController	UMETA(DisplayName = "MotionController"),
	RDT_Unique				UMETA(DisplayName = "Unique Device")
};

USTRUCT(BlueprintType)
struct FRequenceInputAction
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Song")	FString ActionName = "";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Song")	FString KeyAsString = "";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Song")	FKey Key;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Song")	uint32 bShift : 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Song")	uint32 bCtrl : 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Song")	uint32 bAlt : 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Song")	uint32 bCmd : 1;

	FRequenceInputAction() {}
	FRequenceInputAction(const FString InActionName, const FKey InKey, const bool bInShift, const bool bInCtrl, const bool bInAlt, const bool bInCmd)
		: Key(InKey), KeyAsString(Key.GetDisplayName().ToString()), bShift(bInShift), bCtrl(bInCtrl), bAlt(bInAlt), bCmd(bInCmd)
	{
		ActionName = InActionName;
	}

	FRequenceInputAction(const FInputActionKeyMapping& Action)
		: Key(Action.Key), KeyAsString(Action.Key.GetDisplayName().ToString()), bShift(Action.bShift), bCtrl(Action.bCtrl), bAlt(Action.bAlt), bCmd(Action.bCmd)
	{
		ActionName = Action.ActionName.ToString();
	}
};

USTRUCT(BlueprintType)
struct FRequenceInputAxis
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Song")	FString AxisName = "";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Song")	FString KeyAsString = "";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Song")	FKey Key;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Song")	float Scale = 1;

	FRequenceInputAxis() {}
	FRequenceInputAxis(const FString InAxisName, FKey InKey, float InScale)
		: AxisName(InAxisName), KeyAsString(InKey.GetDisplayName().ToString()), Key(InKey), Scale(InScale)
	{ }

	FRequenceInputAxis(const FInputAxisKeyMapping& Axis)
		: Key(Axis.Key), KeyAsString(Axis.Key.GetDisplayName().ToString()), Scale(Axis.Scale)
	{
		AxisName = Axis.AxisName.ToString();
	}
};

/*
*  Impeller Studios (2017)
*  RequenceStructs
*  Contributors: Danny de Bruijne
*
*  Data structs used by Requence
*/
UCLASS()
class URequenceStructs : public UObject
{
	GENERATED_BODY()

	URequenceStructs();

};