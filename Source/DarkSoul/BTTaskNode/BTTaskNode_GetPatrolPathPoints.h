#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"

#include "BTTaskNode_GetPatrolPathPoints.generated.h"

UCLASS()
class DARKSOUL_API UBTTaskNode_GetPatrolPathPoints : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTaskNode_GetPatrolPathPoints(const FObjectInitializer& InObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	int32 Index = 0;

};
