#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Global/Global.h"

#include "C_PowerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKSOUL_API UC_PowerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UC_PowerComponent();

protected:
	virtual void BeginPlay() override;

// ************************************************************************************
		// Method
// ************************************************************************************

private:
	UFUNCTION()
		void LoadPowerInfoFromDataTable();
	
public:
	UFUNCTION()
		bool SendDamage(AActor* DamageCauser, AActor* OtherActor, bool IsSkillDamage = false, float SkillDamage = 0.f);
	UFUNCTION()
		void TakeBuff();

public:
	UFUNCTION()
		void TriggerStatusEffect(EAttributeType InAttributeType);

	void SpawnImpactEffect(const class ACharacter** InCharacter);

// ************************************************************************************
		// Data Member
// ************************************************************************************
	
/*	************************************************************************************
		
	[Remark]	:	Damage Type Class를 Power Component에서 직접 설정하도록 합니다.

	목적:
		Skill 공격일 경우, 각 무기의 AttributeType에 따라서, Status Effect를 Damage와 함께 전달합니다.
	설명:
		1. PowerComponent::DamageTypeClass를 Unreal Editor에서 설정하도록 합니다.
		2. Apply Damage 구현 과정에서 IsSkillDamage == true일 경우, 미리 설정한 Damage Type Class도 함께 전달합니다.

*	************************************************************************************/

private:
	class AWeapon* OwnerWeapon;

private:
	class UDataTable* DataTable_PowerInfo;
	FPowerInfo PowerInfo_Struct;

private:
	TArray<TTuple<FName, bool> > IsImpactEffectSocketNamesActive;

private:
	TMap<EAttributeType, TSubclassOf<class AStatusEffect>> MStatusEffects;
	class AStatusEffect* StatusEffect;

//	************************************************************************************
	// Getter & Setter
//	************************************************************************************

public:
	FORCEINLINE FPowerInfo GetPowerInfoStruct() { return PowerInfo_Struct; }
	FORCEINLINE void SetAttackPower(float InFloat) { PowerInfo_Struct.AttackPower = InFloat; }
	FORCEINLINE void SetIsImpactEffectSocketNamesActive(int InIndex, bool InBool) { IsImpactEffectSocketNamesActive[InIndex].Get<1>() = InBool; }

};
