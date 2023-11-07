#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "Interface/DamageTypeHealthInterface.h"

#include "DT_Debuff.generated.h"

UCLASS()
class DARKSOUL_API UDT_Debuff : public UDamageType, public IDamageTypeHealthInterface
{
	GENERATED_BODY()
	
public:
	virtual void TriggerDebuffStatusEffect(class UC_HealthComponent* OutComp) override;
	virtual void TriggerBuffStatusEffect(class UC_PowerComponent* OutComp, const EAttributeType& InEnum) override {};

	virtual float GetImpulseAmount() { return 0.f; }


};
