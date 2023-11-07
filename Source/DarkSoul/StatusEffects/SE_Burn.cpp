#include "SE_Burn.h"
#include "Global/Global.h"

#include "GameObjects/GameObject.h"
#include "Particles/ParticleSystemComponent.h"

ASE_Burn::ASE_Burn()
{
	
	PrimaryActorTick.bCanEverTick = false;

	// Particle
	GetParticle()->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));

	// �⺻ ����
	IsDebuff = true;

	// �⺻ ����
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

	[Remark]	:	DoT ����

	����:
		Status Effect�� Activate�Ǹ�, Attach Actor���� Damage Over Time�� �����մϴ�.
	����:
		1.	����, SetTimer�� �μ��� ������ �Լ� �����͸� Lambda �������� �ۼ�������, ������ �߻��մϴ�.

			GetWorld()->GetTimerManager().SetTimer(TmpTimerHandler, FTimerDelegate::CreateLambda([&]() {
				SendDamage(AttachActor);
				}), TmpRate, false);

		2.	���ο� ������� FTimerDelegate::CreateUObject()�� ���� FTimerDelegate�� �����մϴ�.
		3.	SetTimer() �Լ��� ���ڷ� FTimerDelegate�� �����ϰ�, ������ �ذ�Ǿ����ϴ�.

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

	[Remark]	:	Attached Mesh Comp�� ��ü���� DoT Damage�� �����մϴ�.

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