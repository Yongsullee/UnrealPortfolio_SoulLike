#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Global/Custom_Structs.h"

#include "HUD_InventorySlot.generated.h"

// #1 Use/Drop Windown -> Use 클릭
DECLARE_DYNAMIC_DELEGATE_OneParam(FUseClicked, FInventoryItemInfo, InUsedItemInfoStruct);

// #0 Use/Drop Windown -> Drop 클릭
DECLARE_DYNAMIC_DELEGATE_OneParam(FDropClicked, FInventoryItemInfo, InDropItemInfoStruct);

// #0 Use/Drop Windown -> 착용중인 아이템이 Inventory로 귀속
DECLARE_DYNAMIC_DELEGATE_OneParam(FAcquireReturnedItem, FInventoryItemInfo, InReturnedItemInfoStruct);

UCLASS()
class DARKSOUL_API UHUD_InventorySlot : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized();

public:
	UFUNCTION()
		void AddExistingItemToSlot();
	UFUNCTION()
		void AddNewItemToSlot(FInventoryItemInfo InStruct);
	UFUNCTION()
		void RemoveItemFromSlot();

	void RefreshItemSlot();

private:
	UFUNCTION()
		void UseItem();
	void UsePotionItem();
	void UseWeaponItem();

	UFUNCTION()
		void DropItem();
	void DropPotionItem(FVector InLocation, FRotator InRotation);
	void DropWeaponItem(FVector InLocation, FRotator InRotation);

private:
	UFUNCTION()
		void OnOffShowDropUseWindow();

private:
	UFUNCTION()
		void OnDescriptionWindow();
	UFUNCTION()
		void OffDescriptionWindow();
	UFUNCTION()
		void ShowDescriptionText();

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* ItemButton;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* ItemImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UEditableTextBox* ItemDescription;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UEditableTextBox* ItemQuantity;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UOverlay* DropUseWindowOverlay;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UHUD_DropUseWindow* DropUseWindow;

public:
	FUseClicked OnUseClicked;
	FDropClicked OnDropClicked;
	FAcquireReturnedItem OnAcquireReturnedItem;

protected:
	UPROPERTY(EditDefaultsOnly)
		class UTexture2D* BlankImage;

	FInventoryItemInfo ItemInfoStruct;
	UPROPERTY(EditDefaultsOnly, Category = "Inventory Slot")
		FInventoryItemInfo NullItemInfoStruct;

	bool bFilled = false;

	bool bDropUseWindowShownOnScreen = false;
	bool bDescriptionWindowShownOnScreen = false;

	int32 Quantity = 0;

private:
	class AWeapon* WeaponItem;

public:

	FORCEINLINE bool IsFilled() { return bFilled; }

	FORCEINLINE int32 GetItemQuantity() { return Quantity; }
	FORCEINLINE void SetItemQuantity(int32 InNum) { Quantity = InNum; }

	FORCEINLINE FInventoryItemInfo GetInventoryItemInfoStruct() { return ItemInfoStruct; }
	FORCEINLINE void SetInventoryItemInfoStruct(FInventoryItemInfo InStruct) { ItemInfoStruct = InStruct; }

	FORCEINLINE UButton* GetInventoryItemButton() { return ItemButton; }
};
