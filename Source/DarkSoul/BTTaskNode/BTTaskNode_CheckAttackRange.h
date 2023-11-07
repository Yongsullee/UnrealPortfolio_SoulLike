#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"

#include "BTTaskNode_CheckAttackRange.generated.h"

UCLASS()
class DARKSOUL_API UBTTaskNode_CheckAttackRange : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTaskNode_CheckAttackRange(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Attack Range")
		float Min=0.f;
	UPROPERTY(EditAnywhere, Category = "Attack Ragne")
		float Max=0.f;

};
