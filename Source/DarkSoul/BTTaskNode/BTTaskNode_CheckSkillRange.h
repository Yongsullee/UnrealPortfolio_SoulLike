#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"

#include "BTTaskNode_CheckSkillRange.generated.h"

UCLASS()
class DARKSOUL_API UBTTaskNode_CheckSkillRange : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskNode_CheckSkillRange(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	float Min = 0.f;
	float Max = 0.f;
	
};
