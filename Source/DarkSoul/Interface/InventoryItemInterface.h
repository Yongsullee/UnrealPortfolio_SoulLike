#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Global/Custom_Structs.h"

#include "InventoryItemInterface.generated.h"

UINTERFACE(MinimalAPI)
class UInventoryItemInterface : public UInterface
{
	GENERATED_BODY()
};

class DARKSOUL_API IInventoryItemInterface
{
	GENERATED_BODY()

public:
	virtual FInventoryItemInfo InteractWithInventory() = 0;

};
