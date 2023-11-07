#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "Global/Custom_Enums.h"

#include "TP_EnemySpawnLocation.generated.h"

UCLASS()
class DARKSOUL_API ATP_EnemySpawnLocation : public ATargetPoint
{
	GENERATED_BODY()

private:

	UPROPERTY(EditAnywhere, Category = "Enemy")
		TSubclassOf<class AEnemyBase> EnemyClass;

public:

	FORCEINLINE TSubclassOf<class AEnemyBase> GetEnemyClass() { return EnemyClass; }
};
