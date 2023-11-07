#include "BackgroundSound.h"
#include "Global/Global.h"

#include "Components/AudioComponent.h"
#include "Space/Portal.h"

ABackgroundSound::ABackgroundSound()
{}

void ABackgroundSound::BeginPlay()
{

	Super::BeginPlay();

	// #1. BG Sounds
	if (SoundCues.Num() > 0)
	{
		for (const auto& SoundCue : SoundCues)
		{
			QBGSoundCues.Enqueue(SoundCue);
		}
	}

	TArray<AActor*> AllActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APortal::StaticClass(), AllActors);
	for (const auto Actor : AllActors)
	{
		APortal* Portal = Cast<APortal>(Actor);
		if (Portal)
		{
			Portal->OnSoundFadeOut.AddDynamic(this, &ABackgroundSound::OnSoundFadeOut);
		}
	}

	SpawnBGSoundCue();

}


void ABackgroundSound::SpawnBGSoundCue()
{

	if (QBGSoundCues.IsEmpty() == false)
	{
		BGAudioComp = UGameplayStatics::SpawnSound2D(GetWorld(), *(QBGSoundCues.Peek()));
		if (!!BGAudioComp)
		{
			BGAudioComp->OnAudioFinished.AddDynamic(this, &ABackgroundSound::OnBGSoundFinished);
			BGAudioComp->bAutoDestroy = false;
		}
		else
		{
			PrintLine();
		}
	}

}

void ABackgroundSound::OnBGSoundFinished()
{

	if (BGAudioComp && bLoopingSoundCue)
	{
		BGAudioComp->Stop();
		USoundCue* PrevSoundCue;
		if (QBGSoundCues.Dequeue(PrevSoundCue))
		{
			QBGSoundCues.Enqueue(PrevSoundCue);
			SpawnBGSoundCue();
		}
	}

}

void ABackgroundSound::OnSoundFadeOut()
{

	if (BGAudioComp)
	{
		BGAudioComp->FadeOut(SoundCueFadeOutDuration, 0.f, EAudioFaderCurve::Linear);

		FTimerHandle AudioStopHandle;
		GetWorld()->GetTimerManager().SetTimer(AudioStopHandle, FTimerDelegate::CreateLambda([&]() {
			if (BGAudioComp)
			{
				bLoopingSoundCue = false;
				BGAudioComp->Stop();
			}
			}), 0.5f, false);
	}

}
