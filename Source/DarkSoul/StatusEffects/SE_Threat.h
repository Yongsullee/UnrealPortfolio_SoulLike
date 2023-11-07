#pragma once

#include "CoreMinimal.h"
#include "StatusEffects/StatusEffect.h"

#include "SE_Threat.generated.h"

UCLASS()
class DARKSOUL_API ASE_Threat : public AStatusEffect
{
	GENERATED_BODY()
	
public:
	ASE_Threat();

protected:
	virtual void BeginPlay() override;

	// ************************************************************************************
			// Method
	// ************************************************************************************

public:
	virtual void StartActivation() override;
	
	virtual void SendDamage(class AActor* InActor) override;

	virtual void AttachComponentToOwnerComponent(class USkeletalMeshComponent* InSkeletalMeshComponent);

	// ************************************************************************************
			// Data Member
	// ************************************************************************************


};
