#include "DT_Buff.h"
#include "Global/Global.h"

#include "ActorComponents/C_PowerComponent.h"


void UDT_Buff::TriggerBuffStatusEffect(UC_PowerComponent* OutComp, const EAttributeType& InEnum)
{

	OutComp->TriggerStatusEffect(InEnum);

}
