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
	
	[Remark]	:	InSkillNum 은 NoComboWeapon에서 Play할 Montage를 결정함과 동시에, 하위 클래스 Magic에서 Spawn할 Projectile 클래스 또한 결정합니다.

	목적:
		기본 공격 혹은 각 스킬 마다 다른 Projectile_Magic 클래스를 Spawn하기 위해 EMagicProjectile 유형의 변수의 값을 변경합니다.
	설명:
		1. 인수로 받은 InSkillNum에 +1을 한 값을 EMagicProjectileType 값으로 변경합니다.
		2. '0' 번 멤버는 기본 공격에 해당하므로, +1을 해줍니다.

*	***********************************************************************************************************************************************/

void AMagic::AttackSkill(const int InSkillNum)
{

	Super::AttackSkill(InSkillNum);

	// #0. Magic Projectile Type 설정
	ProjectileType = static_cast<EProjectileType>(InSkillNum+1);

}

void AMagic::EndAttackSkill()
{

	Super::EndAttackSkill();

	// #0. Magic Projetile Type 변경
	ProjectileType = EProjectileType::E_MagicBall;

}


/*	**************************************************************************************************

	[Remark]	:	Custom Delegate 활용 Weapon < - > Power Component

	목적:
		Weapon의 충돌과 함께 Power Component의 데미지 전달을 연계합니다.
	설명:
		1. Weapon::OnWeaponCollisionBeginOverlap 선언
		2. PowerComponent::SendDamge 함수는 OnWeaponCollisionBeginOverlap과 바인딩을 수행합니다.
		3. 결과적으로, Weapon의 충돌은 PowerComponent의 Damage 전달을 야기합니다.

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