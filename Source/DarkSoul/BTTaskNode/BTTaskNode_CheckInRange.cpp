#include "BTTaskNode_CheckInRange.h"
#include "Global/Global.h"

#include "AIController/AIC_CastleArchery.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTaskNode_CheckInRange::UBTTaskNode_CheckInRange(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Check In Range";
}

EBTNodeResult::Type UBTTaskNode_CheckInRange::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	AAIC_EnemyBase* EnemyAIController = Cast<AAIC_EnemyBase>(OwnerComp.GetAIOwner());
	APawn* OwningPawn = EnemyAIController->GetPawn();
	UBlackboardComponent* BBComponent = EnemyAIController->GetBlackboardComponent();
	AActor* TargetActor = Cast<AActor>(BBComponent->GetValueAsObject("TargetActor"));

	if (!EnemyAIController || !OwningPawn || !BBComponent || !TargetActor) return EBTNodeResult::Failed;

	float Max_SkillRange = EnemyAIController->GetSkillRange().Get<1>();

	float DistanceBetween = FMath::Abs(UKismetMathLibrary::Distance2D(FVector2D(TargetActor->GetActorLocation()), FVector2D(OwningPawn->GetActorLocation())));

	if (DistanceBetween <= Max_SkillRange)
	{
		BBComponent->SetValueAsBool("InRange", true);
	}
	else
	{
		BBComponent->SetValueAsBool("InRange", false);
	}
	
	return EBTNodeResult::Succeeded;

}
