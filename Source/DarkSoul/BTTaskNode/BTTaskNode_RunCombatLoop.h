#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"

#include "BTTaskNode_RunCombatLoop.generated.h"

UCLASS()
class DARKSOUL_API UBTTaskNode_RunCombatLoop : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

protected:
	UBTTaskNode_RunCombatLoop(const FObjectInitializer& InObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	FTimerHandle CombatHandler;
	float CombatRate = 2.5f;

};
