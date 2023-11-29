#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Global/Custom_Structs.h"
#include "Interface/InventoryItemInterface.h"
#include "Interface/DamageTypeHealthInterface.h"

#include "Weapon.generated.h"

/*	****************************************************************************************************************

	[Remark]	:	FourParms -> FiveParams

	목적:
		Weapon의 피격 과정에서 "Staus Effect", 즉 상태 이상 공격을 추가 구현하기 위해 EAttributeType을 인자로 추가합니다.
	설명:
		1. FourParms -> FiveParams 로 교체
		2. C_PowerComponent 에서 Load하는 Weapon의 Power Information 중 "Attribute(속성)" 또한 Damage 전달 구현에 포함합니다.

*	****************************************************************************************************************/

DECLARE_DELEGATE_RetVal_FourParams(bool, FWeaponCollisionBeginOverlap, AActor*, AActor*, bool, float);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBuffStatusEffect);

// #1. Montage Component
DECLARE_DYNAMIC_DELEGATE_FiveParams(FPlayAnimMontage, EStateType, InStateType, EWeaponType, InWeaponType, EComboType, InComboType, uint8, InComboCount, uint8, InSkillIndex);
// #2. Audio Component
DECLARE_DYNAMIC_DELEGATE_SixParams(FPlayWeaponSound, const class AActor*, InActor, const EStateType, InStateType, const EWeaponType, InWeaponType, const EComboType, InComboType, const uint8, InComboCount, const uint8, InSkillIndex);


UCLASS()
class DARKSOUL_API AWeapon : public AActor, public IInventoryItemInterface, public IDamageTypeHealthInterface
{
	GENERATED_BODY()

public:
	AWeapon();

protected:
	virtual void BeginPlay() override;


// ************************************************************************************
		// Method
// ************************************************************************************

public:
	UFUNCTION()
		virtual void AttachComponentToOwnerComponent(USkeletalMeshComponent* InSkeletalMeshComponent);

public:
	UFUNCTION()
		virtual void OnSelected();
	UFUNCTION()
		virtual void OffSelected();

public:

	UFUNCTION()
		bool IsOwnerCanAttack(AWeapon* CurrentWeapon);

	virtual void Attack();
	virtual void EndAttack();

	virtual void AttackSkill(const int InSkillNum);
	virtual void EndAttackSkill();

	void StartAttackDilation();

	virtual void GuardSkill();
	virtual void GuardSkill_Block();
	virtual void EndGuardSkill();

public:
	virtual void OnCollision();
	virtual void OffCollision();

public:
	UFUNCTION()
		virtual void  OnZoneEffect();
		virtual void OffZoneEffect();

public:
	// [23-06-22] Test
	UFUNCTION()
		void PlaySound();


public:
	FWeaponCollisionBeginOverlap OnWeaponCollisionBeginOverlap;
	
	FBuffStatusEffect OnBuffStatusEffect;

	FPlayAnimMontage OnPlayAnimMontage;

	FPlayWeaponSound OnPlayWeaponSound;

// ************************************************************************************
		// Interface Method
// ************************************************************************************
protected:
	virtual FInventoryItemInfo InteractWithInventory() override;

public:
	virtual void TriggerDebuffStatusEffect(class UC_HealthComponent* OutComp) override {};
	virtual void TriggerBuffStatusEffect(class UC_PowerComponent* OutComp, const EAttributeType& InEnum) override {};
	virtual float GetImpulseAmount() override;

// ************************************************************************************
		// Data Member
// ************************************************************************************

private:
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class UC_PowerComponent* PowerComponent;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Weapon");
		class UParticleSystemComponent* ZoneEffect;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
		EWeaponType WeaponType = EWeaponType::E_Unarmed;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
		EComboType ComboType = EComboType::E_Max;

	// [23-11-26] : HUD
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		FWeaponTextureInfo WeaponTextureInfo;

protected:
	UPROPERTY(VisibleAnywhere, category = "Weapon")
		TSet<AActor*> AttackHitActors;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		float Power;
	
	UPROPERTY(VisibleAnywhere)
		bool IsSkillAttack = false;

	uint8 SkillNum = -1;

	UPROPERTY(VisibleAnywhere)
		float SkillBasePower = 0.f;

	float ImpulseAmount = 0.f;

protected:

	UPROPERTY(VisibleAnywhere)
		float ComboCount = 0.f;

	UPROPERTY(VisibleAnywhere)
		bool bExistNextCombo = false;

	UPROPERTY(VisibleAnywhere)
		bool bEnableNextCombo = false;

	UPROPERTY(VisibleAnywhere)
		bool bLastCombo = false;

protected:
	// [23-11-26] : Inventory
	UPROPERTY(EditDefaultsOnly, Category = "Item")
		FInventoryItemInfo ItemInfoStruct;

// ************************************************************************************
		// Getter & Setter
// ************************************************************************************

public:
	FORCEINLINE void SetComboType(EComboType InType) { ComboType = InType; }
	FORCEINLINE void SetImpulseAmount(float InFloat) { ImpulseAmount = InFloat; }
	FORCEINLINE void SetSkillNum(uint8 InNum) { SkillNum = InNum; }
	FORCEINLINE void SetIsSkillAttack(bool InBool) { IsSkillAttack = InBool; }
	FORCEINLINE void SetSkillBasePower(float InFloat) { SkillBasePower = InFloat; }

	FORCEINLINE UC_PowerComponent* GetPowerComponent() { return PowerComponent; }
	FORCEINLINE EWeaponType GetWeaponType() { return WeaponType; }
	FORCEINLINE EComboType GetComboType() { return ComboType; }
	FORCEINLINE float GetComboCount() { return ComboCount; }
	FORCEINLINE FWeaponTextureInfo GetWeaponTextureInfoStruct() { return WeaponTextureInfo; }

};
