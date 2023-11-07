#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Global/Custom_Structs.h"

#include "C_HealthComponent.generated.h"

/*	***************************************************************************************************
	
	[Remark]	:	Player의 HUD 중 Health Bar의 진행 상태를 업데이트합니다.

	목적:
		HUD의 Health Bar Percentage 업데이트
	설명:
		1. HealthComponent	:	Custom Delegate 선언
		2. PlayerCharacter	:	Custom Delegate와 Binding하고, 미리 지정한 HUDProfile의 Health Bar를 Udpate

*	***************************************************************************************************/

// #0. To Owner Object
DECLARE_DYNAMIC_DELEGATE(FStartGuardSkillBlock);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FOwnerIsDamaged, class AActor*, InActor, float, InImpulseAmount, bool, bStatusEffectTriggered);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FOwnerIsDead, class AActor*, InActor, float, InImpulseAmount, bool, bStatusEffectTriggered);

// #1. To HUD Component
DECLARE_DELEGATE_TwoParams(FUpdateHUDProfileHealthBar, float, float);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FUpdateEnemyHUDProfileHealthBar, float, InFloat1, float, InFloat2);

// #2. To Reward Comp
DECLARE_DYNAMIC_DELEGATE(FDropRewardItem);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DARKSOUL_API UC_HealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UC_HealthComponent();

protected:
	virtual void BeginPlay() override;


// ************************************************************************************
		// Method
// ************************************************************************************
	
public:
	UFUNCTION()
		void LoadSpecInfoFromDataTable();

private:
	UFUNCTION()
		void TakeDamage(
			AActor* DamagedActor,
			float Damage,
			const class UDamageType* DamageType,
			class AController* InstigatedBy,
			AActor* DamageCauser
		);

	UFUNCTION()
		void RegenerateCurrentHealth();
	UFUNCTION()
		void DecreaseCurrentHealth(float InFloat);

public:
	UFUNCTION()
		void RecoverCurrentHealth(float InRecoveryAmount);

private:
	UFUNCTION()
		bool HandleBlockDamage(class AActor* InActor);
	UFUNCTION()
		void HandleDamage(class AActor* InActor, bool bStatusEffectTriggered);

public:
	UFUNCTION()
		void TriggerStatusEffect(EAttributeType InAttributeType);


public:
	FStartGuardSkillBlock OnStartGuardSkillBlock;
	FOwnerIsDamaged OnOwnerIsDamaged;
	FOwnerIsDead OnOwnerIsDead;

public:
	FUpdateHUDProfileHealthBar OnUpdateHUDProfileHealthBar;
	FUpdateEnemyHUDProfileHealthBar OnUpdateEnemyHUDProfileHealthBar;

public:
	FDropRewardItem OnDropRewardItem;

// ************************************************************************************
		// Data Member
// ************************************************************************************

private:
	class AGameObject* OwnerObject = nullptr;

private:
	class UDataTable* DataTable_SpecInfo;
	FSpecInfo SpecInfo_Struct;

private:
	UPROPERTY(VisibleAnywhere, Category = "Stat")
		float CurrentHealth = 0.f;
	UPROPERTY(VisibleAnywhere, Category = "Stat")
		float DefensePower = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Mana")
		FTimerHandle RegenHealthHandler;
	UPROPERTY(EditDefaultsOnly, Category = "Mana")
		float HealthRegenRate = 0.f;

private:
	TMap<EAttributeType, TSubclassOf<class AStatusEffect>> MStatusEffects;
	class AStatusEffect* StatusEffect = nullptr;

//	************************************************************************************
		// Getter & Setter
//	************************************************************************************

public:
	FORCEINLINE void SetSpecInfoDefensePower(float InFloat) { SpecInfo_Struct.DefensePower = InFloat; }

	FORCEINLINE FSpecInfo GetSpecInfoStruct() { return SpecInfo_Struct; }
	FORCEINLINE float GetCurrentHealth() { return CurrentHealth; }
	

};
