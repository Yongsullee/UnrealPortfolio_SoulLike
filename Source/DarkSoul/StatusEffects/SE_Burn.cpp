#include "SE_Burn.h"
#include "Global/Global.h"

#include "GameObjects/GameObject.h"
#include "Particles/ParticleSystemComponent.h"

ASE_Burn::ASE_Burn()
{
	
	PrimaryActorTick.bCanEverTick = false;

	// Particle
	GetParticle()->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));

	// 기본 설정
	IsDebuff = true;

	// 기본 설정
	DebuffInfo_Struct.Damage = 5.f;
	DebuffInfo_Struct.MaxDamage = 20.f;
	DebuffInfo_Struct.DamageRate = 1.f;
	DebuffInfo_Struct.IsDoTDamage = true;

	// Life Span
	InitialLifeSpan = 3.0f;

}

void ASE_Burn::BeginPlay()
{
	Super::BeginPlay();

}

/*	***********************************************************************************************

	[Remark]	:	DoT 구현

	목적:
		Status Effect가 Activate되면, Attach Actor에게 Damage Over Time을 전달합니다.
	설명:
		1.	먼저, SetTimer의 인수로 전달할 함수 포인터를 Lambda 형식으로 작성했지만, 에러가 발생합니다.

			GetWorld()->GetTimerManager().SetTimer(TmpTimerHandler, FTimerDelegate::CreateLambda([&]() {
				SendDamage(AttachActor);
				}), TmpRate, false);

		2.	새로운 방법으로 FTimerDelegate::CreateUObject()를 통해 FTimerDelegate를 생성합니다.
		3.	SetTimer() 함수의 인자로 FTimerDelegate를 전달하고, 에러가 해결되었습니다.

*	***********************************************************************************************/

void ASE_Burn::StartActivation()
{

	AActor* AttachActor = GetOwner();
	CheckNull(AttachActor);

	Super::StartActivation();

	// Timer
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &ASE_Burn::SendDamage, AttachActor );
	GetWorld()->GetTimerManager().SetTimer(TimerHandler, TimerDelegate, DebuffInfo_Struct.DamageRate, true);

}

/*	**************************************************************************************************

	[Remark]	:	Attached Mesh Comp의 주체에게 DoT Damage를 전달합니다.

*	**************************************************************************************************/

void ASE_Burn::SendDamage(AActor* InActor)
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

void ASE_Burn::AttachComponentToOwnerComponent(USkeletalMeshComponent* InSkeletalMeshComponent)
{
	Super::AttachComponentToOwnerComponent(InSkeletalMeshComponent);
}