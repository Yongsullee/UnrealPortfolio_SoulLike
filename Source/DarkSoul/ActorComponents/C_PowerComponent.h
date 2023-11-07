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
		
	[Remark]	:	Damage Type Class�� Power Component���� ���� �����ϵ��� �մϴ�.

	����:
		Skill ������ ���, �� ������ AttributeType�� ����, Status Effect�� Damage�� �Բ� �����մϴ�.
	����:
		1. PowerComponent::DamageTypeClass�� Unreal Editor���� �����ϵ��� �մϴ�.
		2. Apply Damage ���� �������� IsSkillDamage == true�� ���, �̸� ������ Damage Type Class�� �Բ� �����մϴ�.

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
