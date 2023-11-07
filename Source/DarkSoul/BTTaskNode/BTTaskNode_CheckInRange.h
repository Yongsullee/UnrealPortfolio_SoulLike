#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"

#include "BTTaskNode_CheckInRange.generated.h"

UCLASS()
class DARKSOUL_API UBTTaskNode_CheckInRange : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTaskNode_CheckInRange(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
