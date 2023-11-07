#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUD/HUD_Direction.h"

#include "HUD_Radar.generated.h"

UCLASS()
class DARKSOUL_API UHUD_Radar : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized();

public:
	UWidgetAnimation* GetAnimationByName(FName AnimationName) const;

	bool PlayAnimationByName(FName AnimationName,
		float StartAtTime,
		int32 NumLoopsToPlay,
		EUMGSequencePlayMode::Type PlayMode,
		float PlaybackSpeed);


protected:
	void FillAnimationsMap();

public:
	UFUNCTION()
		void SetRadarVisibility();

	UFUNCTION()
		void StartBlendOut();
	UFUNCTION()
		void StartBlendIn();

public:
	UFUNCTION()
		void CreateDirectionWidgets(FDirectionInfo InStruct);

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UOverlay* Overlay;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* RadarImage;

protected:
	TMap<FName, UWidgetAnimation*> AnimationsMap;

	bool bVisible = true;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Radar")
		TSubclassOf<UHUD_Direction> DirectionWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Radar")
		FDirectionInfo DirectionInfos[4];

	TArray<class UHUD_Direction*> DirectionWidgets;

};
