#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Global/Global.h"

#include "C_InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKSOUL_API UC_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UC_InventoryComponent();

protected:
	virtual void BeginPlay() override;

public:
	bool IsInventoryShownOnScreen();

public:
	UFUNCTION()
		void CreateInventoryWidget();

	UFUNCTION()
		void ShowInventoryWidgetOnScreen(EItemType InType = EItemType::E_All);
	UFUNCTION()
		void RemoveInventoryWidgetOnScreen();

private:
	UFUNCTION()
		void SwapInventoryWidget(EItemType InType);

public:
	UFUNCTION()
		void AcquireItem();

	bool AcquireConsumableItems(class AActor** OutActor);
	bool AcquireWeaponItems(class AActor** OutActor);

private:
	UFUNCTION()
		void UseItemOccursInAnyInventoryWidget(FInventoryItemInfo InUsedItemInfoStruct, EItemType InInventoryWidgetItemCategory);
	UFUNCTION()
		void DropItemOccursInAnyInventoryWidget(FInventoryItemInfo InDropItemInfoStruct, EItemType InInventoryWidgetItemCategory);

	UFUNCTION()
		void AcquireReturnedItem(FInventoryItemInfo InReturnedItemInfoStruct, EItemType InInventoryWidgetItemCategory);

private:
	bool bShownOnScreen[3] = {0,};

private:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UHUD_Inventory> InventoryWidgetClass;

	UPROPERTY(VisibleAnywhere, Category = "UI")
		TArray<class UHUD_Inventory*> InventoryWidgets;


};
