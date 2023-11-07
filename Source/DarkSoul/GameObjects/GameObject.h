#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Global/Global.h"
#include "Interface/OwnerWeaponInterface.h"

#include "Perception/AISightTargetInterface.h"

#include "GameObject.generated.h"

// #0. State
DECLARE_DYNAMIC_DELEGATE_OneParam(FChangeStateType, EStateType, InType);

// #1. Montage Comp
DECLARE_DYNAMIC_DELEGATE_FiveParams(FPlayHitAnim, EStateType, InStateType, EWeaponType, InWeaponType, EComboType, InComboType, uint8, InComboCount, uint8, InSkillNum);
DECLARE_DYNAMIC_DELEGATE_FiveParams(FPlayDeathAnim, EStateType, InStateType, EWeaponType, InWeaponType, EComboType, InComboType, uint8, InComboCount, uint8, InSkillNum);
DECLARE_DYNAMIC_DELEGATE_FiveParams(FPlayKnockDownAnim, EStateType, InStateType, EWeaponType ,InWeaponType, EComboType, InComboType, uint8 ,InComboCount, uint8 ,InSkillNum);
DECLARE_DYNAMIC_DELEGATE_FiveParams(FPlayWakeUpAnim, EStateType, InStateType, EWeaponType, InWeaponType, EComboType, InComboType, uint8, InComboCount, uint8, InSkillNum);


// #2. Weapon Comp
DECLARE_DYNAMIC_DELEGATE_OneParam(FChangeWeapon, EWeaponType, InType);
DECLARE_DYNAMIC_DELEGATE_OneParam(FChangeClass, EPlayerCombatType, InType);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWeaponAttack);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWeaponEndAttack);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeaponAttackSkill, uint8, InSkillNum);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWeaponEndAttackSkill);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWeaponGuardSkill);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWeaponGuardSkill_Block);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWeaponEndGuardSkill);

// #3. Vault Comp
DECLARE_DYNAMIC_DELEGATE(FRoll);
DECLARE_DYNAMIC_DELEGATE(FDoubleJump);
DECLARE_DYNAMIC_DELEGATE(FStartVault);

// #4. Stamina Comp
DECLARE_DYNAMIC_DELEGATE_OneParam(FStartSprint, bool, IsSprinting);

// #5. Target Comp
DECLARE_DYNAMIC_DELEGATE(FLockOnTarget);
DECLARE_DYNAMIC_DELEGATE(FCycleClockwise);
DECLARE_DYNAMIC_DELEGATE(FCycleAntiClockwise);

// [23-07-01] Sound Event Queue
// #6. Audio Comp
DECLARE_DYNAMIC_DELEGATE_SixParams(FOnPlayHumanSound, const class AActor*, InActor, const EStateType, InStateType, const EWeaponType, InWeaponType, const EComboType, InComboType, const uint8, InComboCount, const uint8, InSkillNum);

UCLASS()
class DARKSOUL_API AGameObject : public ACharacter, public IOwnerWeaponInterface, public IAISightTargetInterface
{
	GENERATED_BODY()

public:
	AGameObject();

protected:
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

//	******************************************************************************************
		// Method
//	******************************************************************************************

public:
	UFUNCTION()
		void StartJump();
	UFUNCTION()
		void EndJump();

public:
	virtual void OnSprint();
	virtual void OffSprint();

public:
	virtual void StartVault();
	virtual void DoubleJump();

public:
	UFUNCTION()
		virtual void Attack();
	UFUNCTION()
		virtual void EndAttack();

	virtual void AttackSkill_00();
	virtual void AttackSkill_01();
	virtual void AttackSkill_02();

	virtual void EndAttackSkill();

public:
	virtual void GuardSkill();

	UFUNCTION()
		virtual void GuardSkill_Block();

	void EndGuardSkill();

public:
	UFUNCTION()
		virtual void IsStartDamaged(class AActor* InDamageCauser, float InImpulseAmount = 0.f, bool bStatusEffectTriggered = false);
	UFUNCTION()
		virtual void IsEndDamaged();
	UFUNCTION()
		virtual void IsDead(class AActor* InDamageCauser, float InImpulseAmount = 0.f, bool bStatusEffectTriggered = false);

	virtual void Respawn(const FVector& InLocation);

public:
	virtual void ChangeColor();
	UFUNCTION()
		virtual void RestoreColor() {}

protected:
	virtual void LookAtDamageCauser(class ACharacter** InCharacter);
	virtual void KnockBack(class ACharacter* InCharacter, int32 InNum);

public:
	UFUNCTION()
		virtual void KnockDown();
	UFUNCTION()
		virtual void StartWakeUp();
	UFUNCTION()
		virtual void IsExpired();

// AI
public:
	bool IsHostile(class AGameObject* InObject);

	virtual bool CanBeSeenFrom(
		const FVector& ObserverLocation, FVector& OutSeenLocation,
		int32& NumberOfLoSChecksPerformed, float& OutSightStrength,
		const AActor* IgnoreActor = nullptr, const bool* bWasVisible = nullptr,
		int32* UserData = nullptr) const;

public:
	// [23-05-01] : Remark AICombatPhase
	virtual void RunAICombatPhase(EAICombatPhase InEnum) {}

	// [23-08-21] : Check Skill Valid On Current AI Combat Phase
	virtual int GetValidSkillInAICombatPhase(int8 InAICombatPhase) { return -1; }

// Delegate
public:
	FChangeStateType OnChangeStateType;

	FChangeClass OnChangeClass;

	FPlayHitAnim OnPlayHitAnim;
	FPlayDeathAnim OnPlayDeathAnim;
	FPlayKnockDownAnim OnPlayKnockDownAnim;
	FPlayWakeUpAnim OnPlayWakeUpAnim;

	FChangeWeapon OnChangeWeapon;

	FWeaponAttack OnWeaponAttack;
	FWeaponEndAttack OnWeaponEndAttack;
	FWeaponAttackSkill OnWeaponAttackSkill;
	FWeaponEndAttackSkill OnWeaponEndAttackSkill;
	FWeaponGuardSkill OnWeaponGuardSkill;
	FWeaponGuardSkill_Block OnWeaponGuardSkill_Block;
	FWeaponEndGuardSkill OnWeaponEndGuardSkill;

	FRoll OnRoll;
	FDoubleJump OnDoubleJump;
	FStartVault OnStartVault;

	FStartSprint OnStartSprint;

	FLockOnTarget OnLockOnTarget;
	FCycleClockwise OnCycleClockwise;
	FCycleAntiClockwise OnCycleAntiClockwise;

	FOnPlayHumanSound OnPlayHumanSound;

// *****************************************************************************
		// Data Member
// *****************************************************************************

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class UC_HealthComponent* HealthComponent;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class UC_ManaComponent* ManaComponent;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class UC_StaminaComponent* StaminaComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
		class UC_StateComponent* StateComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
		class UC_VaultComponent* VaultComponent;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class UArrowComponent* VaultArrow;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class UC_IKComponent* IKComponent;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class UC_WeaponComponent* WeaponComponent;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class UC_MontageComponent* MontageComponent;
	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UC_RewardComponent* RewardComponent;
	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UC_FXComponent* FXComponent;
	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UC_AudioComponent* AudioComponent;

// DA
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Data")
		class UDA_Weapon* DA_Weapon;

//Rate
protected:
	uint8 MaxJumpCount = 2;
	uint8 JumpCount = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate = 45.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate = 45.f;
	
protected:
	const float KnockDownImpulse = 500.f;

protected:
	class UMaterialInstanceDynamic* BloodMaterial;
	FTimerHandle ChangeMaterialHandler;

private:
	FTimerHandle ExpireHandler;
	const float ExpireRate = 1.5f;

// AI
protected:
	class AAIController* AIController;
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		class UBehaviorTree* AIBehaviorTreeAsset;
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
		class ASmartObject* AISmartObject;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
		FName PerceptionTarget = "spine_02";

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
		EFaction Faction = EFaction::E_Neutral;


// *****************************************************************************
		// Interface
// *****************************************************************************

public:
	UFUNCTION(BlueprintPure)
		virtual EStateType GetOwnerStateType() override;
	UFUNCTION(BlueprintCallable)
		virtual void SetOwnerStateType(EStateType StateTypeVal) override;

	UFUNCTION(BlueprintPure)
		virtual EComboType GetOwnerComboType() override;

	UFUNCTION(BlueprintPure)
		virtual AWeapon* GetOwnerCurrentWeapon() override;

//	******************************************************************************************
		// Getter & Setter
//	******************************************************************************************

public:
	FORCEINLINE void SetSmartObject(class ASmartObject* InSmartObject) { AISmartObject = InSmartObject; }

public:
	UFUNCTION(BlueprintPure)
		FORCEINLINE class UC_HealthComponent* GetHealthComponent() { return HealthComponent; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE class UC_ManaComponent* GetManaComponent() { return ManaComponent; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE class UC_StateComponent* GetStateComponent() { return StateComponent; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE class UC_WeaponComponent* GetWeaponComponent() { return WeaponComponent; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE class UC_IKComponent* GetIKComponent() { return IKComponent; }
	FORCEINLINE class UDA_Weapon* GetWeaponDataAsset() { return DA_Weapon; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE uint8 GetJumpCount() { return JumpCount; }
	
	FORCEINLINE UArrowComponent* GetVaultArrowComp() { return VaultArrow; }
	FORCEINLINE class UBehaviorTree* GetBehaviorTreeAsset() { return AIBehaviorTreeAsset; }
	FORCEINLINE class ASmartObject* GetSmartObject() { return AISmartObject; }
	FORCEINLINE EFaction GetFaction() { return Faction; }


};
