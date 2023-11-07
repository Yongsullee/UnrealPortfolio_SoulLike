#include "BTService_SetFocus.h"
#include "Global/Global.h"

#include "AIController/AIC_EnemyBase.h"
#include "BehaviorTree/BlackboardComponent.h"

void UBTService_SetFocus::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	AAIController* AIController = Cast<AAIController>(OwnerComp.GetAIOwner());
	UBlackboardComponent* BBComponent = AIController->GetBlackboardComponent();

	CheckTrue(!AIController || !BBComponent)

	if (AIController->GetBrainComponent()->IsPaused())
	{
		AIController->ClearFocus(EAIFocusPriority::LastFocusPriority);
	}
	else
	{
		// #0. AI State
		if ((uint8)BBComponent->GetValueAsEnum("AIState") >= (uint8)EAIState::E_Attack)
		{
			AIController->ClearFocus(EAIFocusPriority::LastFocusPriority);
		}
		else
		{
			AIController->SetFocus(Cast<AActor>(BBComponent->GetValueAsObject("TargetActor")), EAIFocusPriority::Gameplay);
		}
	}

	// #1. Anim Instance
	UpdateAgentCombatState(OwnerComp);

}

void UBTService_SetFocus::UpdateAgentCombatState(UBehaviorTreeComponent& OwnerComp)
{

	AAIC_EnemyBase* AIController = Cast<AAIC_EnemyBase>(OwnerComp.GetAIOwner());
	UBlackboardComponent* BBComponent = AIController->GetBlackboardComponent();
	CheckTrue(!AIController || !BBComponent);

	EAIState AIState = static_cast<EAIState>(BBComponent->GetValueAsEnum("AIState"));
	CheckTrue((uint8)AIState <= -1);

	AIController->ChangeAgentAIState(AIState);

}
