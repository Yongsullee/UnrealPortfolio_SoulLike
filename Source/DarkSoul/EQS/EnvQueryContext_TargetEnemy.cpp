#include "EnvQueryContext_TargetEnemy.h"
#include "Global/Global.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"

void UEnvQueryContext_TargetEnemy::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{

	Super::ProvideContext(QueryInstance, ContextData);

	AAIController* AIController = Cast<AAIController>((Cast<AActor>((QueryInstance.Owner).Get())->GetInstigatorController()));
	CheckNull(AIController);

	AActor* Target = Cast<AActor>(AIController->GetBlackboardComponent()->GetValueAsObject("TargetActor"));
	CheckNull(Target);

	UEnvQueryItemType_Actor::SetContextHelper(ContextData, Target);

}
