#include "SE_Buff.h"
#include "Global/Global.h"

#include "GameObjects/GameObject.h"
#include "Particles/ParticleSystemComponent.h"

#include "ActorComponents/C_WeaponComponent.h"
#include "ActorComponents/C_PowerComponent.h"
#include "ActorComponents/C_HealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


ASE_Buff::ASE_Buff()
{

	PrimaryActorTick.bCanEverTick = false;

	// Particle
	UParticleSystem* particle;
	CHelpers::GetAsset<UParticleSystem>(&particle, "ParticleSystem'/Game/AuraFX01/Particles/P_ky_aura26'");
	GetParticle()->SetTemplate(particle);
	GetParticle()->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.5f));

	// 버프/디버프 여부
	IsDebuff = false;

	// 기본 설정
	BuffInfo_Struct.Buff = 10.f;
	BuffInfo_Struct.BuffRate = 10.f;
	BuffInfo_Struct.MaxBuff = 30.f;

	// Life Span
	InitialLifeSpan = 10.0f;

}

void ASE_Buff::BeginPlay()
{
	Super::BeginPlay();
}

void ASE_Buff::StartActivation()
{


	AActor* AttachActor = GetOwner();
	AGameObject* AttachObject = Cast<AGameObject>(AttachActor);
	
	CheckTrue(!AttachActor || !AttachObject);

	Super::StartActivation();
	
	// #0. Buff Attack Power
	BuffAttackPower(&AttachObject);

	// #1. Buff Speed
	BuffWalkSpeed(&AttachObject);

	// Timer
	GetWorld()->GetTimerManager().SetTimer(TimerHandler, FTimerDelegate::CreateLambda([&]() {
		EndActivation();
		}), BuffInfo_Struct.BuffRate, false);

}

void ASE_Buff::EndActivation()
{

	if (GetWorld()->GetTimerManager().IsTimerActive(TimerHandler))
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandler);
	}

	AActor* AttachActor = GetOwner();
	if (AttachActor)
	{
		AGameObject* AttachObject = Cast<AGameObject>(AttachActor);
		if (AttachObject)
		{
			Super::EndActivation();
			TArray<AWeapon*> Weapons = AttachObject->GetWeaponComponent()->GetWeapons();
			
			// #0. Weapons Attack Power 감소
			if (Weapons.Num() > 0)
			{
				for (auto Weapon : Weapons)
				{
					if(!!Weapon)
					{
						float OriginalAttackPower = Weapon->GetPowerComponent()->GetPowerInfoStruct().AttackPower;
						Weapon->GetPowerComponent()->SetAttackPower(OriginalAttackPower - BuffInfo_Struct.Buff);
					}
				}
			}

			// #1. Speed 감소
			AttachObject->GetCharacterMovement()->MaxWalkSpeed = 400.f;
		}
	}

}


void ASE_Buff::BuffHealth(AGameObject** OutObject)
{

	if (!(*OutObject))
		return;

	if((*OutObject)->GetHealthComponent())
		(*OutObject)->GetHealthComponent()->RecoverCurrentHealth(10.f);

}

void ASE_Buff::BuffAttackPower(AGameObject** OutObject)
{

	TArray<AWeapon*> Weapons = (*OutObject)->GetWeaponComponent()->GetWeapons();

	CheckTrue(!(*OutObject) || (Weapons.IsValidIndex(0) == false));

	// #0. Weapons Attack Power 감소
	for (auto Weapon : Weapons)
	{
		if (Weapon)
		{
			float OriginalAttackPower = Weapon->GetPowerComponent()->GetPowerInfoStruct().AttackPower;
			Weapon->GetPowerComponent()->SetAttackPower(OriginalAttackPower + BuffInfo_Struct.Buff);
		}
	}

}

void ASE_Buff::BuffWalkSpeed(AGameObject** OutObject)
{

	CheckTrue(!(*OutObject));

	(*OutObject)->GetCharacterMovement()->MaxWalkSpeed = 600.f;

}
