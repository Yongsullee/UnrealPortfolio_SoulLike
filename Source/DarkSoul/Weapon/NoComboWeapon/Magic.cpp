#include "Magic.h"
#include "Global/Global.h"

#include "Components/SphereComponent.h"
#include "ActorComponents/C_PowerComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "ActorComponents/C_ProjectileSpawnerComponent.h"

#include "GameObjects/GameObject.h"

AMagic::AMagic()
{

	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USceneComponent>(this, &Root, TEXT("Root Component"), RootComponent);
	CHelpers::CreateComponent<USphereComponent>(this, &LeftHand, TEXT("Left Hand"), Root);
	CHelpers::CreateComponent<USphereComponent>(this, &RightHand, TEXT("Right Hand"), Root);
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &ZoneEffect, TEXT("Zone Effect"), Root);
	ProjectileSpawner = CreateDefaultSubobject<UC_ProjectileSpawnerComponent>(TEXT("Projectile Spawner"));

	// Sphere Collision
	SphereComponents.Add(LeftHand);
	SphereComponents.Add(RightHand);

	for (auto sphere : SphereComponents)
	{
		sphere->SetSphereRadius(32.f);
		sphere->SetVisibility(false);
		sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// Particle System
	UParticleSystem* particle;
	CHelpers::GetAsset<UParticleSystem>(&particle, "ParticleSystem'/Game/AuraFX01/Particles/P_ky_aura15'");
	ZoneEffect->SetTemplate(particle);
	ZoneEffect->SetRelativeScale3D(FVector(2.f, 2.f, 1.f));
	ZoneEffect->bAutoActivate = false;

	// Weapon Type
	WeaponType = EWeaponType::E_Magic;

	// Magic Projectile Type
	ProjectileType = EProjectileType::E_MagicBall;


}

void AMagic::BeginPlay()
{

	Super::BeginPlay();

	// #0 Projectile Collision Delegate
	TArray<EProjectileType> TmpArray;
	ProjectileSpawner->GetProjectilePool().GenerateKeyArray(TmpArray);

	for (auto& Type : TmpArray)
	{
		ParallelFor(ProjectileSpawner->GetProjectilePool()[Type].Num(), [&](uint8 Idx) {
			ProjectileSpawner->GetProjectilePool()[Type][Idx]->OnProjectileBeginOverlap.BindUFunction(this, "MagicProjectileBeginOverlap");
			});
	}

}


void AMagic::AttachComponentToOwnerComponent(USkeletalMeshComponent* InSkeletalMeshComponent)
{

	Super::AttachComponentToOwnerComponent(InSkeletalMeshComponent);

	LeftHand->AttachToComponent(InSkeletalMeshComponent, FAttachmentTransformRules::KeepRelativeTransform, TEXT("Magic_LeftHand"));
	RightHand->AttachToComponent(InSkeletalMeshComponent, FAttachmentTransformRules::KeepRelativeTransform, TEXT("Magic_RightHand"));
	ZoneEffect->AttachToComponent(InSkeletalMeshComponent, FAttachmentTransformRules::KeepRelativeTransform, TEXT("ZoneEffect"));

}

// Magic_Ball Spawn
void AMagic::Attack()
{

	Super::Attack();

}

void AMagic::EndAttack()
{

	Super::EndAttack();

}

/*	***********************************************************************************************************************************************
	
	[Remark]	:	InSkillNum �� NoComboWeapon���� Play�� Montage�� �����԰� ���ÿ�, ���� Ŭ���� Magic���� Spawn�� Projectile Ŭ���� ���� �����մϴ�.

	����:
		�⺻ ���� Ȥ�� �� ��ų ���� �ٸ� Projectile_Magic Ŭ������ Spawn�ϱ� ���� EMagicProjectile ������ ������ ���� �����մϴ�.
	����:
		1. �μ��� ���� InSkillNum�� +1�� �� ���� EMagicProjectileType ������ �����մϴ�.
		2. '0' �� ����� �⺻ ���ݿ� �ش��ϹǷ�, +1�� ���ݴϴ�.

*	***********************************************************************************************************************************************/

void AMagic::AttackSkill(const int InSkillNum)
{

	Super::AttackSkill(InSkillNum);

	// #0. Magic Projectile Type ����
	ProjectileType = static_cast<EProjectileType>(InSkillNum+1);

}

void AMagic::EndAttackSkill()
{

	Super::EndAttackSkill();

	// #0. Magic Projetile Type ����
	ProjectileType = EProjectileType::E_MagicBall;

}


/*	**************************************************************************************************

	[Remark]	:	Custom Delegate Ȱ�� Weapon < - > Power Component

	����:
		Weapon�� �浹�� �Բ� Power Component�� ������ ������ �����մϴ�.
	����:
		1. Weapon::OnWeaponCollisionBeginOverlap ����
		2. PowerComponent::SendDamge �Լ��� OnWeaponCollisionBeginOverlap�� ���ε��� �����մϴ�.
		3. ���������, Weapon�� �浹�� PowerComponent�� Damage ������ �߱��մϴ�.

*	**************************************************************************************************/

void AMagic::MagicProjectileBeginOverlap(AActor* OtherActor)
{
	
	AGameObject* DamagedObject = Cast<AGameObject>(OtherActor);
	CheckNull(DamagedObject);

	if (AttackHitActors.Contains(OtherActor) != true)
	{
		if (OnWeaponCollisionBeginOverlap.IsBound())
		{
			OnWeaponCollisionBeginOverlap.Execute(GetOwner(), OtherActor, IsSkillAttack, SkillBasePower);

			AttackHitActors.Add(OtherActor);
		}
	}

}