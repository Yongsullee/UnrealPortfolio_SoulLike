#include "AIC_CastleKnight.h"
#include "BehaviorTree/BlackboardComponent.h"

AAIC_CastleKnight::AAIC_CastleKnight()
{

	// [23-05-03] Remark : Skill Range Ãß°¡
	AttackRange = TTuple<float, float>(0.f, 250.f);
	SkillRange = TTuple<float, float>(0.f, 0.f);
}

void AAIC_CastleKnight::BeginPlay()
{

	Super::BeginPlay();

}

void AAIC_CastleKnight::OnPossess(APawn* InPawn)
{

	Super::OnPossess(InPawn);

	// [23-05-01] : Remark AI Combat Phase
	if (!!BBComponent)
	{
		BBComponent->SetValueAsEnum("AICombatPhase", (uint8)EAICombatPhase::E_Max);
	}

}