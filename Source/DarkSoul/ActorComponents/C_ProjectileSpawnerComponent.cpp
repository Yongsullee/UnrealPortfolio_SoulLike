#include "C_ProjectileSpawnerComponent.h"
#include "Global/Global.h"
#include "Runtime/Core/Public/Async/ParallelFor.h"

#include "GameFramework/Character.h"
#include "Projectiles/Projectile.h"

#include "Weapon/NoComboWeapon/NoComboWeapon.h"

UC_ProjectileSpawnerComponent::UC_ProjectileSpawnerComponent()
{

	PrimaryComponentTick.bCanEverTick = false;

}

void UC_ProjectileSpawnerComponent::BeginPlay()
{

	Super::BeginPlay();

	TMap<EProjectileType, FProjectileInfo> MProjectileInfos;

	// #0. Load
	LoadProjectileInfoFromDataTable(MProjectileInfos);

	// #1. Init
	TArray<EProjectileType> TmpArr;
	MProjectileInfos.GenerateKeyArray(TmpArr);
	for (uint8 i = 0; i < TmpArr.Num(); i++)
	{
		TArray<AProjectile*> TmpArray2;
		TmpArray2.Init(nullptr, MProjectileInfos[TmpArr[i]].Max_PoolSize);

		MProjectilePool.Add(TmpArr[i], TmpArray2);
		PoolHeads.Add(TmpArr[i], &MProjectilePool[TmpArr[i]][0]);
	}

	// #2. Pool
	CreatePool(MProjectileInfos);

	// #3. Delegate
	ANoComboWeapon* OwnerNoCombo = Cast<ANoComboWeapon>(GetOwner());
	if (!!OwnerNoCombo)
	{
		OwnerNoCombo->OnProjectileVisibility.BindUFunction(this, "ProjectileVisibility");
	}

	ParallelFor(TmpArr.Num(), [&](uint8 Idx) {
		for (const auto& Projectile : MProjectilePool[TmpArr[Idx]])
		{
			Projectile->OnProjectileEndLaunched.BindUFunction(this, "EndLaunchedProjectile");
		}
		});

}

void UC_ProjectileSpawnerComponent::LoadProjectileInfoFromDataTable(TMap<EProjectileType, FProjectileInfo>& OutMap)
{

	CheckTrue(DataTable_ProjectilesInfo == nullptr);
	// Projectile Info
	TArray<FName>RowNames = DataTable_ProjectilesInfo->GetRowNames();
	for (FName RowName : RowNames)
	{
		const FString ContextString(TEXT("Projetile Info"));
		FProjectileInfo* RowStruct = DataTable_ProjectilesInfo->FindRow<FProjectileInfo>(RowName, ContextString, true);

		CheckNull(RowStruct);

		OutMap.Add(RowStruct->ProjectileType, *RowStruct);
	}

}

void UC_ProjectileSpawnerComponent::CreatePool(TMap<EProjectileType, FProjectileInfo>& OutMap)
{

	AActor* OwnerWeapon = GetOwner();
	CheckNull(OwnerWeapon);

	for (auto& Pair : OutMap)
	{
		for (uint8 i = 0; i < Pair.Value.Max_PoolSize; i++)
		{
			AProjectile* TmpProjectile = GetWorld()->SpawnActor<AProjectile>(Pair.Value.ProjectileClass, FVector::ZeroVector, FRotator::ZeroRotator);

			TmpProjectile->SetOwner(OwnerWeapon);

			TmpProjectile->SetIsPenetrable(Pair.Value.bPenetrable);
			TmpProjectile->SetExpireRate(Pair.Value.ExpireRate);
			TmpProjectile->SetIndex(i);
			TmpProjectile->SetIsActive(false);
			TmpProjectile->SetNext(&MProjectilePool[Pair.Key][(i + 1) % Pair.Value.Max_PoolSize]);
			TmpProjectile->SetActorHiddenInGame(true);

			MProjectilePool[Pair.Key][i] = TmpProjectile;
		}
	}

}

/*	************************************************************************************************************
* 
*	[Remark] :	FAttachmentTransformRules::KeepRelativeTransform vs SnapToTargetNotIncludingScale
* 
*	목적:
*		이미 발사 동작 중인 Projectile이 Attach 되어 있던 Component의 움직임에 따라서 위치가 변하며 날라가는 문제 발생
* 
*	설명:
*		1. FAttachmentTransformRules::KeepRelativeTransform 으로 Attachment Rule을 설정해서 문제가 발생!
*		2. FAttachmentTransformRules::SnapToTargetNotIncludingScale 으로 변경 후 문제 해결
* 
*	************************************************************************************************************/

void UC_ProjectileSpawnerComponent::TeleportProjectileToSpawnLocation(EProjectileType InType, USkeletalMeshComponent** InComponent, const FName& InName)
{

	CheckNull(*(PoolHeads[InType]));

	(*(PoolHeads[InType]))->TeleportTo((*InComponent)->GetSocketLocation(InName), (*InComponent)->GetComponentRotation() + (*InComponent)->GetRelativeRotation());

	(*(PoolHeads[InType]))->SetIsLoaded(true);
	(*(PoolHeads[InType]))->SetActorHiddenInGame(false);

}


void UC_ProjectileSpawnerComponent::AttachProjectile(EProjectileType InType, USkeletalMeshComponent** InComponent, const FName& InName)
{
	
	CheckNull(*(PoolHeads[InType]));

	(*(PoolHeads[InType]))->AttachToComponent((*InComponent), FAttachmentTransformRules::SnapToTargetNotIncludingScale, InName);

	(*(PoolHeads[InType]))->SetIsLoaded(true);
	(*(PoolHeads[InType]))->SetActorHiddenInGame(false);

}


void UC_ProjectileSpawnerComponent::ProjectileVisibility(bool InBool)
{

	TArray<EProjectileType> KeyArrays;
	MProjectilePool.GenerateKeyArray(KeyArrays);
	
	TArray<AProjectile*>* Projectiles;

	if (InBool)
	{
		for (const auto& Key : KeyArrays)
		{
			Projectiles = MProjectilePool.Find(Key);
			for (auto& Projectile : (*Projectiles))
			{
				Projectile->SetActorHiddenInGame(false);
			}
		}
	}
	else
	{
		for (const auto& Key : KeyArrays)
		{
			Projectiles = MProjectilePool.Find(Key);
			for (auto& Projectile : (*Projectiles))
			{
				Projectile->SetActorHiddenInGame(true);
			}
		}
	}

}

void UC_ProjectileSpawnerComponent::StartLaunchProjectile(EProjectileType InType)
{

	CheckTrue((*(PoolHeads[InType])) == nullptr || (*(PoolHeads[InType]))->GetIsActive() == true);

	(*(PoolHeads[InType]))->SetIsActive(true);
	(*(PoolHeads[InType]))->SetIsLoaded(false);
	(*(PoolHeads[InType]))->StartLaunched();
	PoolHeads[InType] = (*(PoolHeads[InType]))->GetNext();

}


void UC_ProjectileSpawnerComponent::EndLaunchedProjectile(EProjectileType InType, uint8 InIndex)
{

	CheckNull(MProjectilePool[InType][InIndex]);

	MProjectilePool[InType][InIndex]->SetIsActive(false);

	if ((*(PoolHeads[InType]))->GetIsLoaded() == true)
	{
		(*(PoolHeads[InType]))->SetNext(&MProjectilePool[InType][InIndex]);
	}
	else
	{
		MProjectilePool[InType][InIndex]->SetNext((*(PoolHeads[InType]))->GetNext());
		PoolHeads[InType] = &MProjectilePool[InType][InIndex];
	}

}
