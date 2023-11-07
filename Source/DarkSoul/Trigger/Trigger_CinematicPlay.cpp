#include "Trigger_CinematicPlay.h"
#include "Global/Global.h"

#include "Components/BoxComponent.h"
#include "LevelSequenceActor.h"

ATrigger_CinematicPlay::ATrigger_CinematicPlay()
{
	PrimaryActorTick.bCanEverTick = false;

	CHelpers::CreateComponent<USceneComponent>(this, &Root, TEXT("Root"));
	CHelpers::CreateComponent<UBoxComponent>(this, &CinematicTriggerBox, TEXT("Cinematic Trigger Box"), Root);

	CinematicTriggerBox->SetBoxExtent(FVector(64.f, 64.f, 64.f));
	CinematicTriggerBox->SetVisibility(false);

}

void ATrigger_CinematicPlay::BeginPlay()
{

	Super::BeginPlay();
	
	// [23-09-08] : Level Sequence And Cinematic
	CinematicTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ATrigger_CinematicPlay::OnOverlapBegin);

}

void ATrigger_CinematicPlay::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	FTimerHandle LevelSeqPlayHandler;
	GetWorld()->GetTimerManager().SetTimer(LevelSeqPlayHandler, FTimerDelegate::CreateLambda([&]() {
		if (PlayCount < Max_PlayCount && LevelSeqActor)
		{
			PlayerLevelSeq();
			PlayCount++;
		}
		}), 2.f, false);

}

void ATrigger_CinematicPlay::PlayerLevelSeq()
{
	if (LevelSeqActor && LevelSeqActor->GetSequencePlayer())
	{
		LevelSeqActor->GetSequencePlayer()->Play();
	}
}


