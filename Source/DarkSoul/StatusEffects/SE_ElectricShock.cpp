#include "SE_ElectricShock.h"
#include "Global/Global.h"

#include "Particles/ParticleSystemComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "AIController.h"
#include "BrainComponent.h"



ASE_ElectricShock::ASE_ElectricShock()
{
	PrimaryActorTick.bCanEverTick = false;

	CHelpers::CreateComponent(this, &HitParticle, TEXT("Hit Particle"), RootComponent);
	HitParticle->bAutoActivate = false;

	// 버프/디버프 여부
	IsDebuff = true;

	// 기본 설정
	DebuffInfo_Struct.Damage = 0.f;
	DebuffInfo_Struct.MaxDamage = 0.f;
	DebuffInfo_Struct.DamageRate = 5.f;
	DebuffInfo_Struct.IsDoTDamage = false;

	InitialLifeSpan = 7.0f;
}

void ASE_ElectricShock::BeginPlay()
{

	Super::BeginPlay();

}

void ASE_ElectricShock::StartActivation()
{

	AActor* AttachActor = GetOwner();
	CheckNull(AttachActor);

	Super::StartActivation();

	if(HitParticle)
		HitParticle->Activate();

	StopCharacterMovement();

	// #3. Timer 
	GetWorld()->GetTimerManager().SetTimer(TimerHandler, FTimerDelegate::CreateLambda([&]() {
		EndActivation();
		}), DebuffInfo_Struct.DamageRate, false);

}

void ASE_ElectricShock::EndActivation()
{

	if (GetWorld()->GetTimerManager().IsTimerActive(TimerHandler))
		GetWorld()->GetTimerManager().ClearTimer(TimerHandler);


	AActor* AttachActor = GetOwner();
	if (AttachActor)
	{
		ACharacter* AttachCharacter = Cast<ACharacter>(AttachActor);
		if (AttachCharacter)
		{
			AAIController* AIController = Cast<AAIController>(AttachCharacter->GetController());
			if (AIController)
			{
				Super::EndActivation();
				AttachCharacter->GetCharacterMovement()->Activate();
				AIController->GetBrainComponent()->ResumeLogic("Agent Is Now Idle");
			}
		}
	}

}

void ASE_ElectricShock::SendDamage(AActor* InActor)
{
	Super::SendDamage(InActor);

}

void ASE_ElectricShock::StopCharacterMovement()
{

	AActor* AttachActor = GetOwner();
	ACharacter* AttachCharacter = Cast<ACharacter>(AttachActor);
	AAIController* AIController = Cast<AAIController>(AttachCharacter->GetController());

	CheckTrue(!AttachActor || !AttachCharacter || !AIController);

	if (!!AttachActor && !!AttachCharacter && !!AIController)
	{
		AttachCharacter->GetCharacterMovement()->Deactivate();
		AIController->GetBrainComponent()->PauseLogic("Agent Is Frozen");
	}

}

void ASE_ElectricShock::AttachComponentToOwnerComponent(USkeletalMeshComponent* InSkeletalMeshComponent)
{
	Super::AttachComponentToOwnerComponent(InSkeletalMeshComponent);

}
