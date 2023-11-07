#include "Bow.h"
#include "Global/Global.h"

#include "Components/SkeletalMeshComponent.h"
#include "ActorComponents/C_ProjectileSpawnerComponent.h"

#include "GameObjects/GameObject.h"

ABow::ABow()
{
	
	PrimaryActorTick.bCanEverTick = true;

	// Components
	CHelpers::CreateComponent(this, &Root, TEXT("Root"));
	CHelpers::CreateComponent(this, &SkeletalMesh, TEXT("Skeletal Mesh"), Root);
	CHelpers::CreateComponent(this, &Capsule, TEXT("Capsule"), SkeletalMesh);

	SkeletalMesh->SetRelativeScale3D(FVector(1.5f, 1.f, 1.f));

	// Animation Blueprint
	TSubclassOf<UAnimInstance> animInstance;
	CHelpers::GetClass<UAnimInstance>(&animInstance, "AnimBlueprint'/Game/Blueprints/Weapons/NoComboWeapon/CastleArchery_Bow/ABP_CastleArchery_Bow.ABP_CastleArchery_Bow_C'");
	SkeletalMesh->SetAnimInstanceClass(animInstance);

	// Projectile Spawner
	ProjectileSpawner = CreateDefaultSubobject<UC_ProjectileSpawnerComponent>(TEXT("Projectile Spawner"));

	// Weapon Type
	WeaponType = EWeaponType::E_Bow;

	// Projectile Type
	ProjectileType = EProjectileType::E_Arrow;

}

void ABow::BeginPlay()
{

	Super::BeginPlay();

	ProjectileSpawner->AttachProjectile(ProjectileType, &SkeletalMesh, BowSocket);

	TArray<EProjectileType> TmpArray;
	ProjectileSpawner->GetProjectilePool().GenerateKeyArray(TmpArray);

	for (auto& Type : TmpArray)
	{
		ParallelFor(ProjectileSpawner->GetProjectilePool()[Type].Num(), [&](uint8 Idx) {
			ProjectileSpawner->GetProjectilePool()[Type][Idx]->OnProjectileBeginOverlap.BindUFunction(this, "ArrowBeginOverlap");
			});
	}

}

void ABow::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);

	if (bStringReleased == true)
	{
		if (StringAlpha <= 0.f) bStringReleased = false;
		else
		{
			StringAlpha = FMath::FInterpTo(StringAlpha, 0.f, DeltaTime, 20.f);
		}
	}

}

void ABow::AttachComponentToOwnerComponent(USkeletalMeshComponent* InSkeletalMeshComponent)
{
	
	Super::AttachComponentToOwnerComponent(InSkeletalMeshComponent);

	SkeletalMesh->AttachToComponent(InSkeletalMeshComponent, FAttachmentTransformRules::KeepRelativeTransform, TEXT("Bow"));

}

void ABow::Attack()
{

	Super::Attack();

}

void ABow::EndAttack()
{

	Super::EndAttack();

}

void ABow::PullString()
{

	// #0. String ����
	StringAlpha = Max_StringAlpha;

}

/* ****************************************************************************************
* 
*	[Remark]	:	AM_ShootAndDraow �� Anim Notify���� ȣ���մϴ�.
* 
*	����:
*		String�� Release�ϰ�, Projectile�� Launch �մϴ�.
* 
*	����:
*		1. String Alpha �� ���� ������ ���� 1 ~ 0 ���� ���� ����
*		2. Projecitle Spawner�� ���� Projectile_Arrow�� Velocity ���� �ο��մϴ�.(�߻�)
* 
*	****************************************************************************************/

void ABow::ReleaseString()
{
	
	// #0. String Alpha �� FInterpTo(���� ����), ����
	CheckFalse(StringAlpha >= 1.f);
	bStringReleased = true;

	// #1. Projectile_Arrow �߻�
	CheckNull(ProjectileSpawner);
	ProjectileSpawner->StartLaunchProjectile(ProjectileType);

}


void ABow::ArrowBeginOverlap(AActor* OtherActor)
{
	
	AGameObject* OwnerObject = Cast<AGameObject>(GetOwner());
	AGameObject* DamagedObject = Cast<AGameObject>(OtherActor);

	CheckTrue(OwnerObject == nullptr || DamagedObject == nullptr);
	CheckTrue(OwnerObject->IsHostile(DamagedObject) == false);

	if (AttackHitActors.Contains(OtherActor) != true)
	{
		if (OnWeaponCollisionBeginOverlap.IsBound())
		{
			OnWeaponCollisionBeginOverlap.Execute(GetOwner(), OtherActor, IsSkillAttack, SkillBasePower);

			AttackHitActors.Add(OtherActor);
		}
	}

}
