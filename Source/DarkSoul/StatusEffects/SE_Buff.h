#pragma once

#include "CoreMinimal.h"
#include "StatusEffects/StatusEffect.h"

#include "SE_Buff.generated.h"

UCLASS()
class DARKSOUL_API ASE_Buff : public AStatusEffect
{
	GENERATED_BODY()
	
public:
	ASE_Buff();

protected:
	virtual void BeginPlay() override;

	// ************************************************************************************
			// Method
	// ************************************************************************************

public:
	virtual void StartActivation() override;
	virtual void EndActivation() override;
	
	void BuffHealth(class AGameObject** OutObject);
	void BuffAttackPower(class AGameObject** OutObject);
	void BuffWalkSpeed(class AGameObject** OutObject);

	// ************************************************************************************
			// Data Member
	// ************************************************************************************
private:
	UPROPERTY(EditDefaultsOnly, Category = "Components");
	class UNiagaraComponent* Particle2;

};
