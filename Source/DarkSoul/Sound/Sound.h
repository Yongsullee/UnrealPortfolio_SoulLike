#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Sound.generated.h"

UCLASS()
class DARKSOUL_API ASound : public AActor
{
	GENERATED_BODY()
	
public:	
	ASound();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

protected:

	UFUNCTION()
		void PlaySoundAtLocation(class USoundCue* InSoundCue, const class AActor* InActor);

	UFUNCTION()
		void AddNewSoundCueToEventQueue(class USoundCue* InSoundCue1, 
			class USoundCue* InSoundCue2, const class AActor* InActor1, const class AActor* InActor2);

protected:

	// [23-06-20] Event Queue
	TQueue<TTuple<class USoundCue*, const class AActor*>> QSoundEvents;

};
