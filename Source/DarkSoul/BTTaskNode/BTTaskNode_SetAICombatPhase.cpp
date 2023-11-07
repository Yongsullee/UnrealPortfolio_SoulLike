#include "BTTaskNode_SetAICombatPhase.h"

#include "AIController/AIC_EnemyBase.h"
#include "AIController/AIManager.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "ActorComponents/C_HealthComponent.h"

UBTTaskNode_SetAICombatPhase::UBTTaskNode_SetAICombatPhase()
{

	bCreateNodeInstance = true;
	NodeName = "Set AI Combat Phase";

}

EBTNodeResult::Type UBTTaskNode_SetAICombatPhase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Controller = Cast<AAIC_EnemyBase>(OwnerComp.GetAIOwner());
	UBlackboardComponent* BBComponent = Controller->GetBlackboardComponent();
	APawn* OwningPawn = Controller->GetPawn();
	UC_HealthComponent* Health = OwningPawn->FindComponentByClass<UC_HealthComponent>();

	if (Controller == nullptr || BBComponent == nullptr || OwningPawn == nullptr || Health == nullptr) 
		return EBTNodeResult::Failed;

	float MaxHealth = Health->GetSpecInfoStruct().MaxHealth;
	float CurrentHealth = Health->GetCurrentHealth();
	EAICombatPhase ChangeCombatPhase;

	if (MaxHealth <= 0.f) return EBTNodeResult::Failed;

	float Percent = (CurrentHealth / MaxHealth) * 100.f;

	if (Percent >= 50.f && Percent <= 70.f)
	{
		ChangeCombatPhase = EAICombatPhase::E_Phase2;
	}
	else if(Percent >= 0.f && Percent <= 50.f)
	{
		ChangeCombatPhase = EAICombatPhase::E_Phase3;
	}
	else
	{
		ChangeCombatPhase = EAICombatPhase::E_Phase1;
	}

	BBComponent->SetValueAsEnum("AICombatPhase", (uint8)ChangeCombatPhase);

	return EBTNodeResult::Succeeded;
}
