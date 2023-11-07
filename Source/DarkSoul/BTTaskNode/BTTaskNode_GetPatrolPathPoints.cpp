#include "BTTaskNode_GetPatrolPathPoints.h"
#include "AIController.h"

#include "Components/SplineComponent.h"
#include "GameObjects/GameObject.h"
#include "AIController/PatrolPath.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"


UBTTaskNode_GetPatrolPathPoints::UBTTaskNode_GetPatrolPathPoints(const FObjectInitializer& InObjectInitializer)
{

	// #0 Node Instancing
	bCreateNodeInstance = true;
	// #1 Node Name
	NodeName = "Get Path Points";

}

EBTNodeResult::Type UBTTaskNode_GetPatrolPathPoints::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	// #0 Blacboard && AIController
	const UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!Blackboard || !AIController) return EBTNodeResult::Failed;
	
	// #1 Pawn
	AGameObject* EnemyBaseRef = Cast<AGameObject>(AIController->GetPawn());
	if (!EnemyBaseRef) return EBTNodeResult::Failed;

	// #2 Smart Obejcts 
	APatrolPath* PatrolRef = Cast<APatrolPath>(EnemyBaseRef->GetSmartObject());
	if(!PatrolRef || PatrolRef->Locations.Num() < 1) return EBTNodeResult::Succeeded;

	// #3 Blackboard Key Set Value
	OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>("MoveToLocation", PatrolRef->Locations[Index]);

	// #4 Index 
	Index = (Index+1) % PatrolRef->Locations.Num();

	return EBTNodeResult::Succeeded;

}
