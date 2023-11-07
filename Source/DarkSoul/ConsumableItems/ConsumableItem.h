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
*	[Remark]	:	Inventory�� ����� Consumable Item�� Ȱ��ȭ ������ �����մϴ�.
*
*	����:
*		Inventory�ȿ� �����ϴ� Consumable Item���� ������ Ư���� ����, "Use" ��ư�� �Բ� Ȱ��ȭ ������ �����մϴ�.
*
*	����:
*		1. Use Button�� �Բ�, Spawn + Activation + Destroy ���� ����
*		2. Activation �Լ��� ������ Item�� Delegate�� ȣ���մϴ�.
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
