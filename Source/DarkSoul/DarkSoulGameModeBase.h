#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DarkSoulGameModeBase.generated.h"

UCLASS()
class DARKSOUL_API ADarkSoulGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ADarkSoulGameModeBase();

protected:
	virtual void BeginPlay() override;

private:
	void SpawnEnemyAtTargetPoints();
	void SpawnWeaponAtTargetPoints();
	void SpawnConsumableItemAtTargetPoints();

private:
	bool bSpawnPointsHasLoaded = false;

};
