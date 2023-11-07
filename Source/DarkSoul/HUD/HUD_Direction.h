#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DataTable.h"

#include "HUD_Direction.generated.h"


USTRUCT()
struct FDirectionInfo : public FTableRowBase
{
public:
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 WorldRotation;
};

UCLASS()
class DARKSOUL_API UHUD_Direction : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct();

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UEditableTextBox* DirectionText;

	UPROPERTY(EditDefaultsOnly, Category = "Direction")
		FDirectionInfo DirectionInfoStruct;

public:
	FORCEINLINE FDirectionInfo GetDirectionInfoStruct() { return DirectionInfoStruct; }
	FORCEINLINE void SetDirectionInfoStruct(FDirectionInfo InStruct) { DirectionInfoStruct = InStruct; }

};
