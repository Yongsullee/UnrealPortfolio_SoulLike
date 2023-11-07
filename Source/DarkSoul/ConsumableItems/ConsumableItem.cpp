#include "ConsumableItem.h"

AConsumableItem::AConsumableItem()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AConsumableItem::BeginPlay()
{
	Super::BeginPlay();
	
}


FInventoryItemInfo AConsumableItem::InteractWithInventory()
{
	return ItemInfoStruct;
}