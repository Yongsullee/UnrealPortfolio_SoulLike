#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Projectiles/Projectile.h"
#include "Global/Custom_Structs.h"

#include "C_ProjectileSpawnerComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKSOUL_API UC_ProjectileSpawnerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UC_ProjectileSpawnerComponent();

protected:
	virtual void BeginPlay() override;

// ************************************************************************************
	// Method
// ************************************************************************************

private:
	void LoadProjectileInfoFromDataTable(TMap<EProjectileType, FProjectileInfo>& OutMap);

public:
	UFUNCTION()
		void CreatePool(TMap<EProjectileType, FProjectileInfo>& OutMap);

	// No Reload
	void TeleportProjectileToSpawnLocation(EProjectileType InType, class USkeletalMeshComponent** InComponent, const FName& InName);

	// Reload
	void AttachProjectile(EProjectileType InType, class USkeletalMeshComponent** InComponent, const FName& InName);

public:
	UFUNCTION()
		void ProjectileVisibility(bool InBool);

public:
	void StartLaunchProjectile(EProjectileType InType);
	UFUNCTION()
		void EndLaunchedProjectile(EProjectileType InType, uint8 InIndex);

// ************************************************************************************
		// Data Member
// ************************************************************************************

private:
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
		class UDataTable* DataTable_ProjectilesInfo;

	TMap<EProjectileType, TArray<AProjectile*>> MProjectilePool;

	TMap<EProjectileType, class AProjectile**> PoolHeads;

public:
	FORCEINLINE TMap<EProjectileType, TArray<AProjectile*>> GetProjectilePool() { return MProjectilePool; }

};
