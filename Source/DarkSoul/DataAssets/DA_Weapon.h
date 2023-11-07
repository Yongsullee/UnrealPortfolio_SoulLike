#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Global/Global.h"

#include "DA_Weapon.generated.h"


UCLASS()
class DARKSOUL_API UDA_Weapon : public UDataAsset
{
	GENERATED_BODY()

public:
	void BeginPlay(class ACharacter* InOwnerCharacter);

public:
	FORCEINLINE TArray<TSubclassOf<class AWeapon>> GetWeaponClasses() { return WeaponClasses; }
	FORCEINLINE class UDataTable* GetHitInfo() { return DataTable_HitInfo; }
	FORCEINLINE class UDataTable* GetDeathInfo() { return DataTable_DeathInfo; }
	FORCEINLINE class UDataTable* GetKnockDownInfo() { return DataTable_KnockDownInfo; }
	FORCEINLINE class UDataTable* GetWakeUpInfo() { return DataTable_WakeUpInfo; }
	FORCEINLINE TArray<class UDataTable*> GetComboType1_Infos() { return DataTables_ComboType1Info; }
	FORCEINLINE TArray<UDataTable*> GetComboType2_Infos() { return DataTables_ComboType2Info; }
	FORCEINLINE TArray<UDataTable*> GetAttackSkillInfos() { return DataTables_AttackSkillInfo; }
	FORCEINLINE TArray<UDataTable*> GetGuardSkillInfos() { return DataTables_GuardSkillInfo; }

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TArray<TSubclassOf<class AWeapon>> WeaponClasses;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		class UDataTable* DataTable_HitInfo;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		class UDataTable* DataTable_DeathInfo;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		class UDataTable* DataTable_KnockDownInfo;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		class UDataTable* DataTable_WakeUpInfo;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TArray<class UDataTable*> DataTables_ComboType1Info;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TArray<class UDataTable*> DataTables_ComboType2Info;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TArray<class UDataTable*>DataTables_AttackSkillInfo;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TArray<class UDataTable*> DataTables_GuardSkillInfo;

};
