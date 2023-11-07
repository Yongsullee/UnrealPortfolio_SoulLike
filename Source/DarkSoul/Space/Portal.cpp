#include "Portal.h"
#include "Global/Global.h"

#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"

#include "GameObjects/PlayerCharacter.h"
#include "ActorComponents/C_WeaponComponent.h"

#include "Engine/LevelStreaming.h"

APortal::APortal()
{
	
	PrimaryActorTick.bCanEverTick = false;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	PortalEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Portal Effect"));
	
	SetRootComponent(Sphere);
	Sphere->InitSphereRadius(350.f);
	Sphere->SetVisibility(true);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	PortalEffect->SetupAttachment(Sphere);
	PortalEffect->bAutoActivate = true;

}

void APortal::BeginPlay()
{

	Super::BeginPlay();

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerController->GetPawn());
		if (PlayerCharacter)
		{
			PlayerCharacter->OnSwitchLevel.AddDynamic(this, &APortal::SwitchLevel);
		}
	}

	CustomLog::Log(NextLevelName);
	
}

// [23-07-07] Level Change
void APortal::SwitchLevel(AActor* InActor)
{

	ULevel* CurrentLevel = GetWorld()->GetCurrentLevel();
	if (CurrentLevel)
	{
		// #1. State 저장
		if (InActor)
		{
			HandOverObjectCurrentState(&InActor);
		}
		// #2. New Level 열기
		FString CurrentLevelPath = GetWorld()->GetCurrentLevel()->GetOutermost()->GetName();
		FString CurrentLevelName = FPaths::GetCleanFilename(CurrentLevelPath).RightChop(9);
		if (!CurrentLevelName.IsEmpty())
		{
			if (!NextLevelName.IsEmpty())
			{
				// #1. Sound Fade Out
				OnSoundFadeOut.Broadcast();

				// #2. Next Level Open
				FTimerHandle LevelSwitchHandle;
				GetWorld()->GetTimerManager().SetTimer(LevelSwitchHandle, FTimerDelegate::CreateLambda([&]() {
					UGameplayStatics::OpenLevel(GetWorld(), FName(NextLevelName));
					}), SoundFadeOutDuration, false);
			}
		}
	}

}

void APortal::HandOverObjectCurrentState(AActor** InActor)
{

	if (!!(*InActor))
	{
		UC_WeaponComponent* WeaponComp = (*InActor)->FindComponentByClass<UC_WeaponComponent>();
		if (WeaponComp)
		{
			TArray<AWeapon*> Weapons = WeaponComp->GetWeapons();
			OnLevelChanged.ExecuteIfBound(Weapons);
		}
	}

}
