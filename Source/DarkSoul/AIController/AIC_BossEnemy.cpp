#include "AIC_BossEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"

AAIC_BossEnemy::AAIC_BossEnemy()
{

	// [23-05-03] Remark : Skill Range Ãß°¡
	AttackRange = TTuple<float, float>(0.f, 450.f);
	SkillRange = TTuple<float, float>(500.f, 1000.f);

}

void AAIC_BossEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void AAIC_BossEnemy::OnPossess(APawn* InPawn)	
{

	Super::OnPossess(InPawn);

	// [23-05-01] : Remark AI Combat Phase
	if (!!BBComponent)
	{
		BBComponent->SetValueAsEnum("AICombatPhase", (uint8)EAICombatPhase::E_Phase1);
	}

}
