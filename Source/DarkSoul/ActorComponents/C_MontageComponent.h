#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Global/Global.h"

#include "C_MontageComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKSOUL_API UC_MontageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UC_MontageComponent();

protected:
	virtual void BeginPlay() override;

private:
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void LoadHitAnimInfoFromDataTable();
	void LoadDeathAnimInfoFromDataTable();
	void LoadKnockDownAnimInfoFromDataTable();
	void LoadWakeUpAnimInfoFromDataTable();
	void LoadComboAnimInfoFromDataTable();
	void LoadAttackSkillAnimInfoFromDataTable();
	void LoadGuardSkillAnimInfoFromDataTable();

public:
	UFUNCTION()
		void PlayMontage(EStateType InStateType, EWeaponType InWeaponType, EComboType InComboType = EComboType::E_Max, uint8 InComboCount = -1, uint8 InSkillNum = -1);
	
private:
	void PlayHitAnimMontage(const EWeaponType& InWeaponType);
	void PlayDeathAnimMontage(const EWeaponType& InWeaponType);
	void PlayKnockDownAnimMontage(const EWeaponType& InWeaponType);
	void PlayWakeUpAnimMontage(const EWeaponType& InWeaponType);

	void PlayAttackAnimMontage(const EWeaponType& InWeaponType, const EComboType& InComboType, uint8 InComboCount);
	void PlayAttackSkillAnimMontage(const EWeaponType& InWeaponType, uint8 InSkillNum);
	void PlayGuardSkillAnimMontage( const EWeaponType& InWeaponType, uint8 InSkillNum);

	void PlayRollAnimMontage();
	void PlayDoubleJumpAnimMontage();
	void PlayParkourAnimMontage();
	void PlayClimbAnimMontage();

private:
	class AGameObject* OwnerObject;
	class UDA_Weapon* DataAsset;

protected:
	TMultiMap<EWeaponType, FCommonAnimInfo> MmHitAnims;
	TMultiMap<EWeaponType, FCommonAnimInfo> MmDeathAnims;
	TMultiMap<EWeaponType, FCommonAnimInfo> MmKnockDownAnims;
	TMultiMap<EWeaponType, FCommonAnimInfo> MmWakeUpAnims;
	TMap < EWeaponType, TArray<FComboAnimInfo*>> MComboType1Anims;
	TMap < EWeaponType, TArray<FComboAnimInfo*>> MComboType2Anims;
	TMap < EWeaponType, TArray<FSkillAnimInfo*>> MAttackSkillAnims;
	TMap < EWeaponType, TArray<FSkillAnimInfo*>> MGuardSkillAnims;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
		class UAnimMontage* RollAnimMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
		class UAnimMontage* DoubleJumpAnimMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
		class UAnimMontage* ClimbAnimMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
		TArray<class UAnimMontage*> ParkourAnimMontages;

};