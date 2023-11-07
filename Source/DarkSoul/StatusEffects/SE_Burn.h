#pragma once

#include "CoreMinimal.h"
#include "StatusEffect.h"

#include "SE_Burn.generated.h"

UCLASS()
class DARKSOUL_API ASE_Burn : public AStatusEffect
{
	GENERATED_BODY()
	
public:	
	ASE_Burn();

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
