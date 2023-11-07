#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Global/Custom_Structs.h"

#include "HUD_Inventory.generated.h"

// #0 Close 버튼
DECLARE_DYNAMIC_DELEGATE(FInventoryHUDCloseButtonClicked);

// #1 Sort Combo Box 옵션 변경 
DECLARE_DYNAMIC_DELEGATE_OneParam(FInventroyHUDSortComboBoxChanged, EItemType, InType);

// #2 Use/Drop Window -> Use 클릭
DECLARE_DYNAMIC_DELEGATE_TwoParams(FUseClickedInInventorySlots, FInventoryItemInfo, InUsedItemInfoStruct, EItemType, InInventoryWidgetItemCategory);

// #3 Use/Drop Window -> Drop 클릭
DECLARE_DYNAMIC_DELEGATE_TwoParams(FDropClickedInInventorySlots, FInventoryItemInfo, InDropItemInfoStruct, EItemType, InInventoryWidgetItemCategory);

// #4 Use/Drop Window -> Use 클릭 -> 착용중인 아이템 Inevntory로 귀속
DECLARE_DYNAMIC_DELEGATE_TwoParams(FInventorySlotAcquireReturnedItem, FInventoryItemInfo, InReturnedItemInfoStruct, EItemType, InInventoryWidgetItemCategory);

UCLASS()
class DARKSOUL_API UHUD_Inventory : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

public:
	bool AddNewItemToInventory(FInventoryItemInfo InStruct);

private:
	bool CheckIfExistingItemOnSlot(FInventoryItemInfo& InStruct);
	void AddExistingItemToSlot(int32 InNum);

private:
	bool CheckIfAnySlotIsEmpty(FInventoryItemInfo& InStruct);
	void AddNewItemToSlot(int32 InNum, FInventoryItemInfo InStruct);

private:
	UFUNCTION()
		void AcquireReturnedItem(FInventoryItemInfo InReturnedItemInfoStruct);

public:
	void ChangeSortComboBoxOption(int32 InNum);

	UFUNCTION()
		void SortInventoryItems(EItemType InType);

private:
	UFUNCTION()
		void ChangeSortCategory(FString InString, ESelectInfo::Type InType);

private:
	UFUNCTION()
		void UseOccursInInventorySlots(FInventoryItemInfo InUsedItemInfoStruct);
	UFUNCTION()
		void DropOccursInInventorySlots(FInventoryItemInfo InDropItemInfoStruct);

private:
	UFUNCTION()
		void FinishInventory();

public:
	FInventoryHUDCloseButtonClicked OnInventoryHUDCloseButtonClicked;
	FInventroyHUDSortComboBoxChanged OnInventroyHUDSortComboBoxChanged;

	FUseClickedInInventorySlots OnUseClickedInInventorySlots;
	FDropClickedInInventorySlots OnDropClickedInInventorySlots;

	FInventorySlotAcquireReturnedItem OnInventorySlotAcquireReturnedItem;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* BGImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UComboBoxString* SortComboBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UHUD_InventorySlot* InventorySlot_1;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UHUD_InventorySlot* InventorySlot_2;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UHUD_InventorySlot* InventorySlot_3;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UHUD_InventorySlot* InventorySlot_4;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UHUD_InventorySlot* InventorySlot_5;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UHUD_InventorySlot* InventorySlot_6;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UEditableTextBox* MoneyText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* CloseButton;


public:
	EItemType ItemCategoryType;

public:
	TArray<UHUD_InventorySlot*> InventorySlots;

public:
	FORCEINLINE TArray< UHUD_InventorySlot*> GetInventorySlots() { return InventorySlots; }
};