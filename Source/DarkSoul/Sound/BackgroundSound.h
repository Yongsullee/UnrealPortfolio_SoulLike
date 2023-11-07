#pragma once

#include "CoreMinimal.h"
#include "Sound/Sound.h"

#include "BackgroundSound.generated.h"

UCLASS()
class DARKSOUL_API ABackgroundSound : public ASound
{
	GENERATED_BODY()
	
public:
	ABackgroundSound();

private:
	void BeginPlay();

private:
	void SpawnBGSoundCue();
	UFUNCTION()
		virtual void OnBGSoundFinished();
	UFUNCTION()
		void OnSoundFadeOut();

private:
	UPROPERTY(EditAnywhere, Category = "Sound")
		TArray<class USoundCue*> SoundCues;

private:
	TQueue<class USoundCue*> QBGSoundCues;
	class UAudioComponent* BGAudioComp;

private:
	const float SoundCueFadeOutDuration = 0.5f;
	bool bLoopingSoundCue = true;

};
