#include "BTTaskNode_OverrideAIState.h"

#include "AIController/AIC_EnemyBase.h"

#include "AIController/AIManager.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Enum.h"

UBTTaskNode_OverrideAIState::UBTTaskNode_OverrideAIState(const FObjectInitializer& ObjectInitializer)
{

	bCreateNodeInstance = true;
	NodeName = "AIState Override";

}

EBTNodeResult::Type UBTTaskNode_OverrideAIState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	AAIC_EnemyBase* Controller = Cast<AAIC_EnemyBase>(OwnerComp.GetAIOwner());
	UBlackboardComponent* BBComponent = Controller->GetBlackboardComponent();
	if (Controller == nullptr || BBComponent == nullptr) return EBTNodeResult::Failed;

	// #0 Target Actor + Target Location 
	AActor* TargetActor;
	FVector TargetLocation;
 
	switch (RunMode)
	{
	case ERunMode::E_ThisAgent:
		BBComponent->SetValue<UBlackboardKeyType_Enum>("AIState", (uint8)DesireState);
		break;
	case ERunMode::E_AllAgent:
		TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
		TargetLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector("MoveToLocation");
		Controller->GetAIManager()->NotifyAIState(DesireState, TargetActor, TargetLocation);
		break;
	}

	return EBTNodeResult::Succeeded;

}
