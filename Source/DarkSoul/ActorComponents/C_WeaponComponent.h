#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Global/Custom_Enums.h"

#include "C_WeaponComponent.generated.h"


// #0. To Mana Comp
DECLARE_DELEGATE_ThreeParams(FRequireMana, int32, float, float);

// #1. To HUD Comp
DECLARE_DELEGATE_OneParam(FUpdateHUDWeaponImage, UTexture2D*);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FUpdateHUDSkillIcons, UTexture2D*, InSkill01Icon, UTexture2D*, InSkill02Icon, UTexture2D*, InSkill03Icon);

// #2. To FX Comp
DECLARE_DYNAMIC_DELEGATE_OneParam(FActivateFX, EWeaponType, InEnum);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKSOUL_API UC_WeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UC_WeaponComponent();

protected:
	virtual void BeginPlay() override;

// *****************************************************************************
		// Method
// *****************************************************************************

private:
	void LoadComboAnimInfoFromDataTable();
	void LoadSkillAnimInfoFromDataTable();

private:
	UFUNCTION()
		void SpawnAttachWeapon();

public:
	UFUNCTION()
		void ChangeWeapon(EWeaponType InType);
	UFUNCTION()
		void ChangeComboType(EComboType InType);

public:
	void EquipWeapon(EWeaponType& InType);
	void UnequipWeapon(EWeaponType& InType);
	void InterchangeWeapon(EWeaponType& InType, EWeaponType& InType2);

	UFUNCTION()
		void UpdateHUDProfile();

public:
	UFUNCTION()
		void SwapWeapon(EWeaponType InType, AWeapon* InWeapon);

private:
	UFUNCTION()
		void Attack();
	UFUNCTION()
		void EndAttack();
	UFUNCTION()
		void AttackSkill(uint8 InSkillNum);
	UFUNCTION()
		void EndAttackSkill();
	UFUNCTION()
		void GuardSkill();
	UFUNCTION()
		void GuardSkill_Block();
	UFUNCTION()
		void EndGuardSkill();

public:
	FRequireMana OnRequireMana;

	FUpdateHUDWeaponImage OnUpdateHUDWeaponImage;
	FUpdateHUDSkillIcons OnUpdateHUDSkillIcons;

	FActivateFX OnActivateFX;

// *****************************************************************************
		// Data Member
// *****************************************************************************
private:
	class AGameObject* OwnerObject;
	class UDA_Weapon* DataAsset;

private:
	EWeaponType WeaponType = EWeaponType::E_Unarmed;
	EPlayerCombatType CombatType = EPlayerCombatType::E_Unarmed;

private:
	TArray<class AWeapon*> Weapons;
	class AWeapon* CurrentWeapon;

private:
	TMap < EWeaponType, TArray<FComboAnimInfo*>> MComboType1Structs;
	TMap < EWeaponType, TArray<FComboAnimInfo*>> MComboType2Structs;
	TMap < EWeaponType, TArray<FSkillAnimInfo*>> MAttackSkillStructs;
	TMap < EWeaponType, TArray<FSkillAnimInfo*>> MGuardSkillStructs;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		class UTexture2D* BlankImage;


// *****************************************************************************
		// Getter&Setter
// *****************************************************************************

public:
	UFUNCTION(BlueprintPure)
		FORCEINLINE EWeaponType GetWeaponType() { return WeaponType;}
	
	FORCEINLINE TArray<class AWeapon*> GetWeapons() { return Weapons; }

	UFUNCTION(BlueprintPure)
		FORCEINLINE EPlayerCombatType GetCombatType() { return CombatType; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE AWeapon* GetCurrentWeapon() { return CurrentWeapon; }

};
