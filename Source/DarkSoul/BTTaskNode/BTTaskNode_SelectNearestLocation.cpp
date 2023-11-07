#include "BTTaskNode_SelectNearestLocation.h"
#include "Global/Global.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

#include "AIController.h"
#include "GameObjects/GameObject.h"

UBTTaskNode_SelectNearestLocation::UBTTaskNode_SelectNearestLocation()
{
	bCreateNodeInstance = true;
	NodeName = "Select Nearest Location";
}

EBTNodeResult::Type UBTTaskNode_SelectNearestLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	AIController = Cast<AAIController>(OwnerComp.GetAIOwner());
	AGameObject* PawnObject = Cast<AGameObject>(AIController->GetPawn());

	if (!AIController || !PawnObject || !LocationSeekerQuery) return EBTNodeResult::Failed;

	LocationSeekerQueryRequest = FEnvQueryRequest(LocationSeekerQuery, PawnObject);
	LocationSeekerQueryRequest.Execute(EEnvQueryRunMode::AllMatching, this, &UBTTaskNode_SelectNearestLocation::EnemySeekerQueryFinished);

	return EBTNodeResult::Succeeded;

}

void UBTTaskNode_SelectNearestLocation::EnemySeekerQueryFinished(TSharedPtr<FEnvQueryResult> Result)
{

	uint8 Index = 0;
	float CurrentBestScore = -1.f;
	TArray<FVector> Locations;
	Result->GetAllAsLocations(Locations);

	for (uint8 i =0; i<Locations.Num(); i++)
	{
		if (Result->GetItemScore(i) > CurrentBestScore)
		{
			NearestLocation = Locations[i];
			CurrentBestScore = Result->GetItemScore(i);
		}
	}

	AIController->GetBlackboardComponent()->SetValueAsVector("MoveToLocation", NearestLocation);

}
