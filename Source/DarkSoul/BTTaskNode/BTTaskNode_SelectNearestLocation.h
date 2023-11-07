#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "EnvironmentQuery/EnvQueryManager.h"

#include "BTTaskNode_SelectNearestLocation.generated.h"

UCLASS()
class DARKSOUL_API UBTTaskNode_SelectNearestLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTaskNode_SelectNearestLocation();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	void EnemySeekerQueryFinished(TSharedPtr<FEnvQueryResult> Result);

private:
	class AAIController* AIController;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard")
		UEnvQuery* LocationSeekerQuery;

	FEnvQueryRequest LocationSeekerQueryRequest;

	FVector NearestLocation = FVector::ZeroVector;

};
