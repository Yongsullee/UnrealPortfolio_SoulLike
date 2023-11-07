#pragma once

#include "CoreMinimal.h"
#include "GameObjects/EnemyBase.h"

#include "BossEnemy.generated.h"

UCLASS()
class DARKSOUL_API ABossEnemy : public AEnemyBase
{
	GENERATED_BODY()

public:
	ABossEnemy();

protected:
	virtual void BeginPlay() override;


protected:
	virtual void IsDead(class AActor* InDamageCauser, float InImpulseAmount = 0.f, bool bStatusEffectTriggered = false) override;

protected:
	virtual void AICombatPhase01() {}
	virtual void AICombatPhase02() {}
	virtual void AICombatPhase03() {}

};
