#include "AIC_CastleArchery.h"
#include "BehaviorTree/BlackboardComponent.h"


AAIC_CastleArchery::AAIC_CastleArchery()
{

	// [23-05-03] Remark : Skill Range Ãß°¡
	AttackRange = TTuple<float, float>(500.f, 800.f);
	SkillRange = TTuple<float, float>(0.f, 0.f);

}

void AAIC_CastleArchery::BeginPlay()
{

	Super::BeginPlay();

}

void AAIC_CastleArchery::OnPossess(APawn* InPawn)
{

	Super::OnPossess(InPawn);

	// [23-05-01] : Remark AI Combat Phase
	if (!!BBComponent)
	{
		BBComponent->SetValueAsEnum("AICombatPhase", (uint8)EAICombatPhase::E_Max);
	}

}