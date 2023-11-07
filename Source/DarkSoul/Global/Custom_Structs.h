#pragma once

#include "CoreMinimal.h"
#include "Custom_Enums.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Animation/AnimMontage.h"
#include "Particles/ParticleSystem.h"

#include "Custom_Structs.generated.h"

UCLASS()
class DARKSOUL_API ACustom_Structs : public AActor
{
	GENERATED_BODY()
};

// 0. Weapon				******************************************************

USTRUCT(Atomic, BlueprintType)
struct FComboAnimInfo : public FTableRowBase
{

public:
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimMontage* AnimMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName StartSection;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float PlayRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool CanMove;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ImpulseAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundCue* WeaponSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundCue* HumanSound;

};

USTRUCT(Atomic, BlueprintType)
struct FSkillAnimInfo : public FTableRowBase
{
public:
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UTexture2D* SkillIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimMontage* AnimMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName StartSection;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float PlayRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool CanMove;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ImpulseAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BaseDamage;	// 스킬 데미지	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ManaRequired;	// 스킬 데미지	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CoolTimeRequired;	// 스킬 데미지	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UNiagaraSystem* GhostEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float EffectDuration = 1.2f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundCue* WeaponSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundCue* HumanSound;

};

// 1. Statics				******************************************************

USTRUCT(Atomic, BlueprintType)
struct FSpecInfo : public FTableRowBase
{
public:
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AttackPower;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DefensePower;
};

// 2. Power					******************************************************

USTRUCT(Atomic, BlueprintType)
struct FPowerInfo : public FTableRowBase
{

public:
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EWeaponType Weapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EAttributeType Attribute;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UDamageType> DamageTypeClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UNiagaraSystem* TrailEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UNiagaraSystem* ImpactEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AttackPower;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float AttackRate = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ImpulseAmount = 200.f;

};

// 3. PlayerCharacter		******************************************************

USTRUCT(Atomic, BlueprintType)
struct FCommonAnimInfo : public FTableRowBase
{

public:
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimMontage* AnimMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName StartSection;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float PlayRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool CanMove;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UParticleSystem* Effect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USoundCue* HumanSound;

};

// 3. Weapon				******************************************************

USTRUCT(Atomic, BlueprintType)
struct FWeaponTextureInfo : public FTableRowBase
{
public:
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* WeaponImage;
};

USTRUCT(Atomic, BlueprintType)
struct FProjectileInfo : public FTableRowBase
{
public:
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EProjectileType ProjectileType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AProjectile> ProjectileClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bPenetrable = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ExpireRate = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bUsingObjectPool = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Max_PoolSize;

};


// 3. Item					******************************************************

USTRUCT(Atomic, BlueprintType)
struct FInventoryItemInfo : public FTableRowBase
{
public:
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EItemType ItemType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EWeaponType WeaponType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EWeaponRank WeaponRank;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor> ItemClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* ItemImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsStackable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString ItemDescription;
};