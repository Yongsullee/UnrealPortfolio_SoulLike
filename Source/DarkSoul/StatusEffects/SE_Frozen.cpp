#include "SE_Frozen.h"
#include "Global/Global.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "BrainComponent.h"

#include "Particles/ParticleSystemComponent.h"

#include "Materials/MaterialInstanceConstant.h"


ASE_Frozen::ASE_Frozen()
{
	PrimaryActorTick.bCanEverTick = false;

	CHelpers::CreateComponent(this, &HitParticle, TEXT("Hit Particle"), RootComponent);
	HitParticle->bAutoActivate = false;

	OriginalMaterials.Init(nullptr, 10);

	// �⺻ ����
	IsDebuff = true;

	// �⺻ ����
	DebuffInfo_Struct.Damage = 0.f;
	DebuffInfo_Struct.MaxDamage = 0.f;
	DebuffInfo_Struct.DamageRate = 5.f;
	DebuffInfo_Struct.IsDoTDamage = false;

	// Life Span
	InitialLifeSpan = 7.0f;
}

void ASE_Frozen::BeginPlay()
{
	Super::BeginPlay();
}

void ASE_Frozen::StartActivation()
{

	AActor* AttachActor = GetOwner();
	CheckNull(AttachActor);

	Super::StartActivation();

	// #1. Change Material
	ChangeMaterialFrozen();

	// #2. Stop Movement
	StopCharacterMovement();

	// #3. Timer 
	GetWorld()->GetTimerManager().SetTimer(TimerHandler, FTimerDelegate::CreateLambda([&]() {
		EndActivation();
		}), DebuffInfo_Struct.DamageRate, false);

}

void ASE_Frozen::EndActivation()
{

	AActor* AttachActor = GetOwner();

	if(AttachActor)
	{ 
		ACharacter* AttachCharacter = Cast<ACharacter>(AttachActor);
		if (AttachCharacter)
		{
			AAIController* AIController = Cast<AAIController>(AttachCharacter->GetController());
			if (AIController)
			{
				Super::EndActivation();

				if (HitParticle) HitParticle->Activate();

				if (OriginalMaterials.Num() > 0)
				{
					for (int32 i = 0; i < AttachCharacter->GetMesh()->GetNumMaterials(); i++)
					{
						if (OriginalMaterials[i])
						{
							AttachCharacter->GetMesh()->SetMaterial(i, OriginalMaterials[i]);
							OriginalMaterials[i] = nullptr;
						}
					}
				}

				AttachCharacter->GetCharacterMovement()->Activate();
				AIController->GetBrainComponent()->ResumeLogic("Agent Is Now Idle");

			}
		}
	}

}

void ASE_Frozen::SendDamage(AActor* InActor)
{
	Super::SendDamage(InActor);
}

/*	**************************************************************************************************

	[Remark]	:	���� �̻� ������ ���� ���� Material�� �־��� �ð����� ������ ������ IceMaterial�� �����մϴ�. 
		
	����:
		Status Effect�� Ȱ��ȭ �ð� ���� �ǰ� ����� Material�� Ice Material�� ��õ��� �����մϴ�.
	����:
		1. Ice Material ����
		2. Original Material�� ��� ���� �����մϴ�.
		3. Status Effect�� Ȱ��ȭ �ð� ���� �ǰ� ����� Material�� Ice Material�� �����մϴ�.
		4. Status Effect�� Ȱ��ȭ �ð��� ����Ǹ�, �ǰ� ����� Material�� �̸� ������ Originla Material�� �ٽ� �����մϴ�.

*	**************************************************************************************************/

void ASE_Frozen::ChangeMaterialFrozen()
{

	AActor* AttachActor = GetOwner();
	ACharacter* AttachCharacter = Cast<ACharacter>(AttachActor);

	CheckTrue(!AttachActor || !AttachCharacter);

	UMaterialInstanceConstant* frozen;
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&frozen, "MaterialInstanceConstant'/Game/Assets/Assets_Materials/M_Ice_Inst.M_Ice_Inst'");

	UMaterialInstanceDynamic* Frozen = UMaterialInstanceDynamic::Create(frozen, this);

	if (AttachCharacter && Frozen)
	{
		int32 MaterialNum = AttachCharacter->GetMesh()->GetNumMaterials();
		for (int32 i = 0; i < MaterialNum; i++)
		{
			if (AttachCharacter->GetMesh()->GetMaterial(i))
			{
				OriginalMaterials[i] = AttachCharacter->GetMesh()->GetMaterial(i);
				AttachCharacter->GetMesh()->SetMaterial(i, Frozen);
			}
		}
	}


}

void ASE_Frozen::StopCharacterMovement()
{

	AActor* AttachActor = GetOwner();
	ACharacter* AttachCharacter = Cast<ACharacter>(AttachActor);
	AAIController* AIController = Cast<AAIController>(AttachCharacter->GetController());

	CheckTrue(!AttachActor || !AttachCharacter || !AIController)

	if (!!AttachActor && !!AttachCharacter && !!AIController)
	{
		AttachCharacter->GetCharacterMovement()->Deactivate();
		AIController->GetBrainComponent()->PauseLogic("Agent Is Frozen");
	}
	else
	{
		PrintLine();
	}

}
