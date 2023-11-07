#pragma once

#include "CoreMinimal.h"
#include "StatusEffects/StatusEffect.h"

#include "SE_Debuff.generated.h"

UCLASS()
class DARKSOUL_API ASE_Debuff : public AStatusEffect
{
	GENERATED_BODY()
	
public:
	ASE_Debuff();
protected:
	virtual void BeginPlay() override;

// ************************************************************************************
		// Method
// ************************************************************************************

public:
	virtual void StartActivation() override;
	virtual void EndActivation() override;

	virtual void SendDamage(class AActor* InActor) override;

	void DecreaseDefensePower(class AGameObject** OutObject);
	void ResetDefensePower(class AGameObject** OutObject);

	virtual void AttachComponentToOwnerComponent(class USkeletalMeshComponent* InSkeletalMeshComponent);

// ************************************************************************************
		// Data Member
// ************************************************************************************
private:
	UPROPERTY(EditDefaultsOnly, Category = "Components");
		class UNiagaraComponent* Particle2;

};
