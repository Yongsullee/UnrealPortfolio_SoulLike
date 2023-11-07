
#include "DT_Burn.h"
#include "Global/Global.h"

#include "ActorComponents/C_HealthComponent.h"

void UDT_Burn::TriggerDebuffStatusEffect(UC_HealthComponent* OutComp)
{
	OutComp->TriggerStatusEffect(EAttributeType::E_Fire);
}

