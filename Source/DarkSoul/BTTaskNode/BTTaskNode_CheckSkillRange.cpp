#include "BTTaskNode_CheckSkillRange.h"
#include "Global/Global.h"

#include "AIController/AIC_CastleArchery.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTaskNode_CheckSkillRange::UBTTaskNode_CheckSkillRange(const FObjectInitializer& ObjectInitializer)
{

	bCreateNodeInstance = true;
	NodeName = "Check Skill Range";

}

// [23-05-03] Remark : Skill Range & Attack Range
EBTNodeResult::Type UBTTaskNode_CheckSkillRange::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	AAIC_EnemyBase* EnemyAIController = Cast<AAIC_EnemyBase>(OwnerComp.GetAIOwner());
	APawn* OwningPawn = EnemyAIController->GetPawn();
	UBlackboardComponent* BBComponent = EnemyAIController->GetBlackboardComponent();
	AActor* TargetActor = Cast<AActor>(BBComponent->GetValueAsObject("TargetActor"));

	if (!EnemyAIController || !OwningPawn || !BBComponent || !TargetActor) return EBTNodeResult::Failed;

	float Min_SkillRange = EnemyAIController->GetSkillRange().Get<0>();
	float Max_SkillRange = EnemyAIController->GetSkillRange().Get<1>();

	float Min_AttackRange = EnemyAIController->GetAttackRange().Get<0>();
	float Max_AttackRange = EnemyAIController->GetAttackRange().Get<1>();

	float DistanceBetween = FMath::Abs(UKismetMathLibrary::Distance2D(FVector2D(TargetActor->GetActorLocation()), FVector2D(OwningPawn->GetActorLocation())));

	// #1. Not In Range Both
	if (DistanceBetween > Max_SkillRange || DistanceBetween < Min_AttackRange)
	{
		BBComponent->SetValueAsBool("InSkillRange", false);
		BBComponent->SetValueAsBool("InAttackRange", false);
	}
	// #2. In Skill Range, but Not In Attack Range
	if (DistanceBetween >= Min_SkillRange && DistanceBetween <= Max_SkillRange)
	{
		BBComponent->SetValueAsBool("InSkillRange", true);
		BBComponent->SetValueAsBool("InAttackRange", false);
	}
	// #3. Not In Skill Range, but In Attack Range
	if(DistanceBetween >= Min_AttackRange && DistanceBetween <= Max_AttackRange)
	{
		BBComponent->SetValueAsBool("InSkillRange", false);
		BBComponent->SetValueAsBool("InAttackRange", true);
	}

	return EBTNodeResult::Succeeded;

}
