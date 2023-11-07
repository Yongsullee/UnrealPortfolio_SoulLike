#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InventoryItemInterface.h"
#include "Global/Global.h"

#include "ConsumableItem.generated.h"

UCLASS()
class DARKSOUL_API AConsumableItem : public AActor, public IInventoryItemInterface
{
	GENERATED_BODY()
	
public:	
	AConsumableItem();

protected:
	virtual void BeginPlay() override;

/*	*********************************************************************************
*
*	[Remark]	:	Inventory에 저장된 Consumable Item의 활성화 동작을 정의합니다.
*
*	목적:
*		Inventory안에 존재하는 Consumable Item들은 각각의 특성에 따라서, "Use" 버튼과 함께 활성화 동작을 수행합니다.
*
*	설명:
*		1. Use Button과 함께, Spawn + Activation + Destroy 동작 수행
*		2. Activation 함수는 각각의 Item의 Delegate를 호출합니다.
*		3. HP -> HealthComponent, MP -> ManaComponent, SP -> StaminaComponent
*
*	*********************************************************************************/


public:
	virtual void StartActivation(class APlayerCharacter** OutPlayer){};

protected:
	virtual FInventoryItemInfo InteractWithInventory() override;

protected:
	 //Must Be Set!!
	UPROPERTY(EditDefaultsOnly, Category = "Item")
		FInventoryItemInfo ItemInfoStruct;
};
