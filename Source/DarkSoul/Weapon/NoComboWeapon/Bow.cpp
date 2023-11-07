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

	// #0. String 증가
	StringAlpha = Max_StringAlpha;

}

/* ****************************************************************************************
* 
*	[Remark]	:	AM_ShootAndDraow 의 Anim Notify에서 호출합니다.
* 
*	목적:
*		String을 Release하고, Projectile을 Launch 합니다.
* 
*	설명:
*		1. String Alpha 값 선형 보간을 통해 1 ~ 0 으로 점차 감소
*		2. Projecitle Spawner를 통해 Projectile_Arrow에 Velocity 값을 부여합니다.(발사)
* 
*	****************************************************************************************/

void ABow::ReleaseString()
{
	
	// #0. String Alpha 값 FInterpTo(선형 보간), 감소
	CheckFalse(StringAlpha >= 1.f);
	bStringReleased = true;

	// #1. Projectile_Arrow 발사
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
