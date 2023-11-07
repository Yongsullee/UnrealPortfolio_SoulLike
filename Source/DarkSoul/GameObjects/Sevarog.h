#pragma once

#include "CoreMinimal.h"
#include "GameObjects/BossEnemy.h"

#include "Sevarog.generated.h"

UCLASS()
class DARKSOUL_API ASevarog : public ABossEnemy
{
	GENERATED_BODY()
	
public:
	ASevarog();

protected:
	virtual void BeginPlay() override;

public:
	virtual void RunAICombatPhase(EAICombatPhase InEnum) override;

private:
	// [23-08-21] : Check Skill Valid On Current AI Combat Phase
	virtual int GetValidSkillInAICombatPhase(int8 InAICombatPhase) override;

	virtual void AICombatPhase01() override;
	virtual void AICombatPhase02() override;
	virtual void AICombatPhase03() override;

private:
	const EEnemyType EnemyType = EEnemyType::E_Sevarog;

};
