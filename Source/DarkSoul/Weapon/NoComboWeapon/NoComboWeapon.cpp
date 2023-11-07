#include "NoComboWeapon.h"
#include "Global/Global.h"

#include "ActorComponents/C_ProjectileSpawnerComponent.h"

#include "GameFramework/Character.h"

ANoComboWeapon::ANoComboWeapon()
{

	PrimaryActorTick.bCanEverTick = false;

	ComboType = EComboType::E_NoCombo;

}

void ANoComboWeapon::BeginPlay()
{

	Super::BeginPlay();

	// 0. Visibility
	OnProjectileVisibility.ExecuteIfBound(false);

}

void ANoComboWeapon::OnSelected()
{
	
	Super::OnSelected();

}

void ANoComboWeapon::OffSelected()
{

	Super::OffSelected();

}

void ANoComboWeapon::Attack()
{

	if (IsOwnerCanAttack(this))
	{

		Super::Attack();

		// #1. Anim Montage -> Montage Component
		OnPlayAnimMontage.ExecuteIfBound(EStateType::E_Attacking, WeaponType, ComboType, ComboCount, SkillNum);

	}
	else
	{
		return;
	}

}

void ANoComboWeapon::LaunchProjectile()
{

	CheckTrue(!ProjectileSpawner);
	ProjectileSpawner->StartLaunchProjectile(ProjectileType);

}

