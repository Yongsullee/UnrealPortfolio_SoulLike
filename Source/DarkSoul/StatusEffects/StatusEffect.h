#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "StatusEffect.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FStatusEffectStartActivation, UTexture2D*, InStatusIcon);
DECLARE_DELEGATE(EndStatusEffectActivation);

USTRUCT(Atomic, BlueprintType)
struct FDebuffInfo : public FTableRowBase
{
public:
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Status Effect")
		float Damage = 0.f;
	UPROPERTY(EditAnywhere, Category = "Status Effect")
		float MaxDamage = 0.f;
	UPROPERTY(EditAnywhere, Category = "Status Effect")
		float DamageRate = 0.0f;
	UPROPERTY(EditAnywhere, Category = "Status Effect")
		bool IsDoTDamage = false;
};

USTRUCT(Atomic, BlueprintType)
struct FBuffInfo : public FTableRowBase
{
public:
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Status Effect")
		EStaticsOnStatusEffect OnWhichStatics = EStaticsOnStatusEffect::E_Max;
	UPROPERTY(EditAnywhere, Category = "Status Effect")
		float Buff = 0.f;
	UPROPERTY(EditAnywhere, Category = "Status Effect")
		float MaxBuff = 0.f;
	UPROPERTY(EditAnywhere, Category = "Status Effect")
		float BuffRate = 0.f;
};


UCLASS()
class DARKSOUL_API AStatusEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	AStatusEffect();

protected:
	virtual void BeginPlay() override;

// ************************************************************************************
		// Method
// ************************************************************************************

public:
	UFUNCTION()
		virtual void StartActivation();
	UFUNCTION()
		virtual void EndActivation();

	EndStatusEffectActivation OnEndStatusEffectActivation;

	UFUNCTION()
		virtual void SendDamage(class AActor* InActor){}
	UFUNCTION()
		virtual void SendBuffEffect(class AActor* InActor){}

	virtual void AttachComponentToOwnerComponent(class USkeletalMeshComponent* InSkeletalMeshComponent);

public:
	FStatusEffectStartActivation OnStatusEffectStartActivation;

// ************************************************************************************
		// Data Member
// ************************************************************************************

private:
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class USceneComponent* Root;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class UParticleSystemComponent* Particle;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class UTextRenderComponent* Text;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Icon")
		class UTexture2D* StatusIcon;

protected:
	FTimerHandle TimerHandler;;

	bool IsDebuff = false;

	FDebuffInfo DebuffInfo_Struct;
	FBuffInfo BuffInfo_Struct;

public:
	FORCEINLINE UTexture2D* GetStatusIcon() { return StatusIcon; }
	FORCEINLINE UParticleSystemComponent* GetParticle() { return Particle; }
	FORCEINLINE bool GetIsDebuff() { return IsDebuff; }


};
