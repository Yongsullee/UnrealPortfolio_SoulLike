#include "DT_Debuff.h"
#include "Global/Global.h"

#include "ActorComponents/C_HealthComponent.h"

void UDT_Debuff::TriggerDebuffStatusEffect(UC_HealthComponent* OutComp)
{
	OutComp->TriggerStatusEffect(EAttributeType::E_Steel);
}

