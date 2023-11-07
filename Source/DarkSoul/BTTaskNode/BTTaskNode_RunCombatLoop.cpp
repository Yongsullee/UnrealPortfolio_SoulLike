#include "BTTaskNode_RunCombatLoop.h"

#include "AIController/AIC_EnemyBase.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "GameObjects/GameObject.h"
#include "Interface/OwnerWeaponInterface.h"

UBTTaskNode_RunCombatLoop::UBTTaskNode_RunCombatLoop(const FObjectInitializer& InObjectInitializer)
{

	bCreateNodeInstance = true;
	NodeName = "Attack";

}

EBTNodeResult::Type UBTTaskNode_RunCombatLoop::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	AAIC_EnemyBase* AIController = Cast<AAIC_EnemyBase>(OwnerComp.GetAIOwner());
	UBlackboardComponent* BBComponent = OwnerComp.GetBlackboardComponent();
	AGameObject* OwningObject = Cast<AGameObject>(AIController->GetPawn());

	if (!AIController || !BBComponent || !OwningObject) return EBTNodeResult::Failed;

	EAICombatPhase Phase = static_cast<EAICombatPhase>(BBComponent->GetValueAsEnum("AICombatPhase"));

	if (Phase > EAICombatPhase::E_Max || Phase < EAICombatPhase::E_Phase1)
	{
		return EBTNodeResult::Failed;
	}
	else if (Phase == EAICombatPhase::E_Max)
	{
		AIController->Attack();
	}
	else
	{
		bool IsSkillRange = BBComponent->GetValueAsBool("InSkillRange");
		bool IsAttackRange = BBComponent->GetValueAsBool("InAttackRange");

		if (IsSkillRange == true && IsAttackRange == false)
		{
			AIController->RunAICombatPhase(Phase);
		}
		else if (IsSkillRange == false && IsAttackRange == true)
		{
			AIController->Attack();
		}

	}

	return EBTNodeResult::Succeeded;

}