#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"

#include "BTTaskNode_IsSkillValid.generated.h"

UCLASS()
class DARKSOUL_API UBTTaskNode_IsSkillValid : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
protected:
	UBTTaskNode_IsSkillValid(const FObjectInitializer& InObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
