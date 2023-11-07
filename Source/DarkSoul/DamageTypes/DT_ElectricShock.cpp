#include "DT_ElectricShock.h"
#include "Global/Global.h"

#include "ActorComponents/C_HealthComponent.h"

void UDT_ElectricShock::TriggerDebuffStatusEffect(UC_HealthComponent* OutComp)
{
	OutComp->TriggerStatusEffect(EAttributeType::E_Electric);
}