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

	����:
		Human Sound�� Weapon Sound�� Audio Component�� ���� �޾ƿ� ����մϴ�.
	����:
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

