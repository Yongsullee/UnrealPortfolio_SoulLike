#pragma once

#include "CoreMinimal.h"
#include "Weapon/Weapon.h"
#include "Global/Custom_Structs.h"

#include "NoComboWeapon.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FProjectileVisibility, bool, InBool);

UCLASS()
class DARKSOUL_API ANoComboWeapon : public AWeapon
{
	GENERATED_BODY()
	
public:
	ANoComboWeapon();

protected:
	virtual void BeginPlay() override;

public:
	virtual void OnSelected() override;
	virtual void OffSelected() override;

public:
	virtual void Attack() override;

public:
	UFUNCTION()
		virtual void LaunchProjectile();

public:
	FProjectileVisibility OnProjectileVisibility;

// ************************************************************************************
		// Member
// ************************************************************************************
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class UC_ProjectileSpawnerComponent* ProjectileSpawner;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Projectile")
		EProjectileType ProjectileType = EProjectileType::E_Max;

// ************************************************************************************
		// Getter & Setter
// ************************************************************************************
public:
	FORCEINLINE class UC_ProjectileSpawnerComponent* GetProjectileSpawnerComponent() { return ProjectileSpawner; }
	FORCEINLINE EProjectileType GetProjectileType() { return ProjectileType; }

};
