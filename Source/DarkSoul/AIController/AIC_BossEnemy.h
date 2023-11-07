#pragma once

#include "CoreMinimal.h"
#include "AIController/AIC_EnemyBase.h"

#include "AIC_BossEnemy.generated.h"

UCLASS()
class DARKSOUL_API AAIC_BossEnemy : public AAIC_EnemyBase
{
	GENERATED_BODY()

public:
	AAIC_BossEnemy();

//	******************************************************************************************

	// Method

//	******************************************************************************************

protected:
	virtual void BeginPlay() override;

private:
	virtual void OnPossess(class APawn* InPawn) override;

};
