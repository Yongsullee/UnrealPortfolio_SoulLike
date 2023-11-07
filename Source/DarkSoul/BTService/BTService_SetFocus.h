#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"

#include "BTService_SetFocus.generated.h"

UCLASS()
class DARKSOUL_API UBTService_SetFocus : public UBTService_BlackboardBase
{

	GENERATED_BODY()

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	void UpdateAgentCombatState(UBehaviorTreeComponent& OwnerComp);

};
