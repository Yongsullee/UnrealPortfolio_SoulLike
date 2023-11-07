#pragma once

#include "CoreMinimal.h"
#include "StatusEffects/StatusEffect.h"

#include "SE_ElectricShock.generated.h"

UCLASS()
class DARKSOUL_API ASE_ElectricShock : public AStatusEffect
{
	GENERATED_BODY()

public:
	ASE_ElectricShock();
protected:
	virtual void BeginPlay() override;

// ************************************************************************************
	// Method
// ************************************************************************************

public:
	virtual void StartActivation() override;
	virtual void EndActivation() override;

	virtual void SendDamage(class AActor* InActor) override;
	virtual void StopCharacterMovement();

	virtual void AttachComponentToOwnerComponent(class USkeletalMeshComponent* InSkeletalMeshComponent);

// ************************************************************************************
	// Data Member
// ************************************************************************************
private:
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class UParticleSystemComponent* HitParticle;

};
