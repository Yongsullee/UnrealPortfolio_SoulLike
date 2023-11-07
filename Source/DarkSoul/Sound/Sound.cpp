#include "Sound.h"
#include "Global/Global.h"

ASound::ASound()
{

	PrimaryActorTick.bCanEverTick = true;

}

void ASound::BeginPlay()
{

	Super::BeginPlay();

}

void ASound::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);

}


void ASound::PlaySoundAtLocation(USoundCue* InSoundCue, const AActor* InActor)
{

	if (!!InSoundCue && !!InActor)
	{
		UGameplayStatics::PlaySoundAtLocation(
			InActor->GetWorld(),
			InSoundCue,
			InActor->GetActorLocation()
		);
	}

}


// [23-06-30] Sound Event Queue
/*	************************************************************************

	목적:
		Human Sound와 Weapon Sound를 Audio Component로 부터 받아와 재생합니다.
	설명:
		InSoundCue1 = Human Sound
		InSoundCue2 = Weapon Sound

*	************************************************************************/

void ASound::AddNewSoundCueToEventQueue(USoundCue* InSoundCue1, USoundCue* InSoundCue2,
	const AActor* InActor1, const AActor* InActor2)
{

	if (!!InSoundCue1 && !!InActor1)
	{
		QSoundEvents.Enqueue(MakeTuple(InSoundCue1, InActor1));
	}

	if (!!InSoundCue2 && !!InActor2)
	{
		QSoundEvents.Enqueue(MakeTuple(InSoundCue2, InActor2));
	}

}

