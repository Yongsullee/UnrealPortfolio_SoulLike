#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"

#include "BTTaskNode_SetAICombatPhase.generated.h"

UCLASS()
class DARKSOUL_API UBTTaskNode_SetAICombatPhase : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskNode_SetAICombatPhase();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
