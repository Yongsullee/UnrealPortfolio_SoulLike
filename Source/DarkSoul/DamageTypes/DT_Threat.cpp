#include "DT_Threat.h"
#include "Global/Global.h"

#include "ActorComponents/C_HealthComponent.h"

void UDT_Threat::TriggerDebuffStatusEffect(UC_HealthComponent* OutComp)
{
	OutComp->TriggerStatusEffect(EAttributeType::E_Dark);
}