#pragma once

#include "CoreMinimal.h"
#include "StatusEffects/StatusEffect.h"

#include "SE_Frozen.generated.h"

UCLASS()
class DARKSOUL_API ASE_Frozen : public AStatusEffect
{
	GENERATED_BODY()
	
public:
	ASE_Frozen();
protected:
	virtual void BeginPlay() override;

// ************************************************************************************
		// Method
// ************************************************************************************

public:
	virtual void StartActivation() override;
	virtual void EndActivation() override;

	virtual void SendDamage(class AActor* InActor) override;

	void ChangeMaterialFrozen();
	virtual void StopCharacterMovement();

// ************************************************************************************
		// Data Member
// ************************************************************************************

private:
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class UParticleSystemComponent* HitParticle;

private:
	TArray<class UMaterialInterface*> OriginalMaterials;

};
