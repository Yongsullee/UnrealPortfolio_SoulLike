#include "BTTaskNode_CheckAttackRange.h"
#include "Global/Global.h"

#include "AIController/AIC_CastleArchery.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTaskNode_CheckAttackRange::UBTTaskNode_CheckAttackRange(const FObjectInitializer& ObjectInitializer)
{

	bCreateNodeInstance = true;
	NodeName = "Check Attack Range";

}

EBTNodeResult::Type UBTTaskNode_CheckAttackRange::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	AAIC_EnemyBase* EnemyAIController = Cast<AAIC_EnemyBase>(OwnerComp.GetAIOwner());
	APawn* OwningPawn = EnemyAIController->GetPawn();
	UBlackboardComponent* BBComponent = EnemyAIController->GetBlackboardComponent();
	AActor* TargetActor = Cast<AActor>(BBComponent->GetValueAsObject("TargetActor"));

	if(!EnemyAIController || !OwningPawn || !BBComponent || !TargetActor) return EBTNodeResult::Failed;

	float Min_AttackRange = EnemyAIController->GetAttackRange().Get<0>();
	float Max_AttackRange = EnemyAIController->GetAttackRange().Get<1>();

	float DistanceBetween = FMath::Abs(UKismetMathLibrary::Distance2D(FVector2D(TargetActor->GetActorLocation()), FVector2D(OwningPawn->GetActorLocation())));

	if (DistanceBetween > Max_AttackRange || DistanceBetween < Min_AttackRange)
	{
		BBComponent->SetValueAsBool("InAttackRange", false);
	}
	else
	{
		BBComponent->SetValueAsBool("InAttackRange", true);
	}

	return EBTNodeResult::Succeeded;
}
