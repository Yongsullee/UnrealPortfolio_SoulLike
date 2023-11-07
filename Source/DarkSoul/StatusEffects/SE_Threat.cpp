#include "SE_Threat.h"
#include "Global/Global.h"

#include "GameObjects/GameObject.h"
#include "Particles/ParticleSystemComponent.h"

ASE_Threat::ASE_Threat()
{

	PrimaryActorTick.bCanEverTick = false;

	// 기본 설정
	IsDebuff = true;

	DebuffInfo_Struct.Damage = 5.f;
	DebuffInfo_Struct.MaxDamage = 20.f;
	DebuffInfo_Struct.DamageRate = 1.f;
	DebuffInfo_Struct.IsDoTDamage = true;

	// Life Span
	InitialLifeSpan = 4.0f;

}

void ASE_Threat::BeginPlay()
{
	Super::BeginPlay();
}

void ASE_Threat::StartActivation()
{

	AActor* AttachActor = GetOwner();
	CheckNull(AttachActor);

	Super::StartActivation();

	// Timer
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &ASE_Threat::SendDamage, AttachActor);
	GetWorld()->GetTimerManager().SetTimer(TimerHandler, TimerDelegate, DebuffInfo_Struct.DamageRate, true);

}

void ASE_Threat::SendDamage(AActor* InActor)
{

	if (InActor)
	{
		float damage = UGameplayStatics::ApplyDamage(
			InActor,
			DebuffInfo_Struct.Damage,
			NULL,
			this,
			UDamageType::StaticClass()
		);
	}
	else
	{
		return;
	}

	Super::SendDamage(InActor);

}

void ASE_Threat::AttachComponentToOwnerComponent(USkeletalMeshComponent* InSkeletalMeshComponent)
{
	Super::AttachComponentToOwnerComponent(InSkeletalMeshComponent);
}
