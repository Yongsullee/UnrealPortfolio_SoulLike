#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"

#include "TP_ConsumableItemSpawnLocation.generated.h"

UCLASS()
class DARKSOUL_API ATP_ConsumableItemSpawnLocation : public ATargetPoint
{
	GENERATED_BODY()
	

private:

	UPROPERTY(EditAnywhere, Category = "ConsumableItem")
		TSubclassOf<class AConsumableItem> ConsumableItemClass;

public:

	FORCEINLINE TSubclassOf<class AConsumableItem> GetConsumableItemClass() { return ConsumableItemClass; }
};
