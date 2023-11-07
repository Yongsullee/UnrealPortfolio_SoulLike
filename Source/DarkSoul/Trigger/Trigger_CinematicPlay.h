#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Trigger_CinematicPlay.generated.h"

UCLASS()
class DARKSOUL_API ATrigger_CinematicPlay : public AActor
{
	GENERATED_BODY()
	
public:	
	ATrigger_CinematicPlay();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	void PlayerLevelSeq();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class USceneComponent* Root;
	UPROPERTY(EditAnywhere, Category = "Components")
		class UBoxComponent* CinematicTriggerBox;

public:
	UPROPERTY(EditAnywhere, Category = "Components")
		class ALevelSequenceActor* LevelSeqActor;

private:
	const uint8 Max_PlayCount = 1;
	uint8 PlayCount = 0;

};
