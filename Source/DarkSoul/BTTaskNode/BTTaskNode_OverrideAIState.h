#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Global/Global.h"

#include "BTTaskNode_OverrideAIState.generated.h"


UENUM(BlueprintType)
enum class ERunMode : uint8
{
	E_ThisAgent			UMETA(DisplayName = "This Agent"),
	E_AllAgent			UMETA(DisplayName = "All Agent"),
};

UCLASS()
class DARKSOUL_API UBTTaskNode_OverrideAIState : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTaskNode_OverrideAIState(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
		EAIState DesireState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
		ERunMode RunMode;

};
