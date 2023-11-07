#pragma once

#include "CoreMinimal.h"
#include "GameObjects/EnemyBase.h"

#include "Enemy.generated.h"


UCLASS()
class DARKSOUL_API AEnemy : public AEnemyBase
{
	GENERATED_BODY()

public:
	AEnemy();

protected:
	virtual void BeginPlay() override;


public:
	virtual void IsDead(class AActor* InDamageCauser, float InImpulseAmount = 0.f, bool bStatusEffectTriggered = false) override;

};
