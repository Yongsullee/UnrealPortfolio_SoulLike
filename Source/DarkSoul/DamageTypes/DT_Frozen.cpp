#include "DT_Frozen.h"
#include "Global/Global.h"

#include "ActorComponents/C_HealthComponent.h"

void UDT_Frozen::TriggerDebuffStatusEffect(UC_HealthComponent* OutComp)
{
	OutComp->TriggerStatusEffect(EAttributeType::E_Ice);
}
