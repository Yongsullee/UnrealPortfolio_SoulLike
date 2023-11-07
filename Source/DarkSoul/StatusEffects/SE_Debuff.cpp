#include "SE_Debuff.h"
#include "Global/Global.h"

#include "GameObjects/GameObject.h"
#include "ActorComponents/C_HealthComponent.h"

#include "NiagaraComponent.h"

ASE_Debuff::ASE_Debuff()
{
	PrimaryActorTick.bCanEverTick = false;

	// Niagara 
	CHelpers::CreateComponent<UNiagaraComponent>(this, &Particle2, TEXT("Particle2"), RootComponent);
	Particle2->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.5f));
	Particle2->bAutoActivate = false;

	// 기본 설정
	IsDebuff = true;

	// 기본 설정
	DebuffInfo_Struct.Damage = 0.f;
	DebuffInfo_Struct.MaxDamage = 0.f;
	DebuffInfo_Struct.DamageRate = 3.f;
	DebuffInfo_Struct.IsDoTDamage = false;

	// Life Span
	InitialLifeSpan = 5.0f;
}

void ASE_Debuff::BeginPlay()
{
	Super::BeginPlay();
}

void ASE_Debuff::StartActivation()
{

	AActor* AttachActor = GetOwner();
	AGameObject* AttachObject = Cast<AGameObject>(AttachActor);

	CheckTrue(!AttachActor || !AttachObject);

	Super::StartActivation();

	DecreaseDefensePower(&AttachObject);

	GetWorld()->GetTimerManager().SetTimer(TimerHandler, FTimerDelegate::CreateLambda([&]() {
		EndActivation();
		}), DebuffInfo_Struct.DamageRate, false);

}

void ASE_Debuff::EndActivation()
{

	if (GetWorld()->GetTimerManager().IsTimerActive(TimerHandler))
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandler);
	}

	AActor* AttachActor = GetOwner();
	AGameObject* AttachObject = Cast<AGameObject>(AttachActor);

	CheckTrue(!AttachActor || !AttachObject);

	Super::EndActivation();
	ResetDefensePower(&AttachObject);

}

void ASE_Debuff::SendDamage(AActor* InActor)
{
	Super::SendDamage(InActor);
}


void ASE_Debuff::DecreaseDefensePower(AGameObject** OutObject)
{
	// 0. Defense Power Decreased by 80% ( 4 / 5 )
	float OriginalDefensePower = (*OutObject)->GetHealthComponent()->GetSpecInfoStruct().DefensePower;
	float FinalDefensePower = OriginalDefensePower * 0.8f;

	(*OutObject)->GetHealthComponent()->SetSpecInfoDefensePower(FinalDefensePower);
}

void ASE_Debuff::ResetDefensePower(AGameObject** OutObject)
{
	float OriginalDefensePower = (*OutObject)->GetHealthComponent()->GetSpecInfoStruct().DefensePower;
	float FinalDefensePower = OriginalDefensePower * 1.25f;

	// 0. Defense Power Increased by 125% ( 5 / 4 )
	(*OutObject)->GetHealthComponent()->SetSpecInfoDefensePower(FinalDefensePower);
}

void ASE_Debuff::AttachComponentToOwnerComponent(USkeletalMeshComponent* InSkeletalMeshComponent)
{
	Super::AttachComponentToOwnerComponent(InSkeletalMeshComponent);
}
