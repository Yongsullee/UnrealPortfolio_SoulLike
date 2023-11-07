#include "HUD_InventorySlot.h"
#include "Global/Global.h"

#include "Components/Image.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/Overlay.h"

#include "HUD_DropUseWindow.h"

#include "GameObjects/PlayerCharacter.h"
#include "ActorComponents/C_InventoryComponent.h"
#include "HUD_Inventory.h"

#include "ConsumableItems/ConsumableItem.h"
#include "Weapon/Weapon.h"
#include "ActorComponents/C_WeaponComponent.h"

void UHUD_InventorySlot::NativeOnInitialized()
{

	Super::NativeOnInitialized();

	ItemInfoStruct = NullItemInfoStruct;

	// #1 Drop Use Not Shown
	CheckNull(DropUseWindow);
	DropUseWindow->SetVisibility(ESlateVisibility::Hidden);

	// #2 On & Off Drop Use Window
	ItemButton->OnClicked.AddDynamic(this, &UHUD_InventorySlot::OnOffShowDropUseWindow);

	// #3 On & Off Description Window
	ItemButton->OnHovered.AddDynamic(this, &UHUD_InventorySlot::OnDescriptionWindow);
	ItemButton->OnUnhovered.AddDynamic(this, &UHUD_InventorySlot::OffDescriptionWindow);

	// #4 On Use Button Activation 
	DropUseWindow->OnUseButtonClicked.AddDynamic(this, &UHUD_InventorySlot::UseItem);
	// #5 On Drop Button Activation
	DropUseWindow->OnDropButtonClicked.AddDynamic(this, &UHUD_InventorySlot::DropItem);

}

// #1 Add Existing Item
void UHUD_InventorySlot::AddExistingItemToSlot()
{
	CheckFalse(bFilled);

	// #0 Quantity
	int32 PrevQuantity = FCString::Atoi(*ItemQuantity->GetText().ToString());
	Quantity = PrevQuantity + 1;
	FString NewQuantityStr = FString::FromInt(++PrevQuantity);
	ItemQuantity->SetText(FText::FromString(NewQuantityStr));

}

// #2 Add New Item
void UHUD_InventorySlot::AddNewItemToSlot(FInventoryItemInfo InStruct)
{

	// #0 ItemInfo Struct
	ItemInfoStruct = InStruct;

	// #1 Image
	ItemImage->SetBrushFromTexture(ItemInfoStruct.ItemImage);
	// #2 Description
	ItemDescription->SetText(FText::FromString(ItemInfoStruct.ItemDescription));
	// #3 Quantity
	ItemQuantity->SetText(FText::FromString("1"));
	Quantity = 1;
	// #4 IsFilled?
	bFilled = true;

}


void UHUD_InventorySlot::RemoveItemFromSlot()
{
	CheckFalse(bFilled);

	int32 PrevQuantity = FCString::Atoi(*ItemQuantity->GetText().ToString());
	int32 NextQuantity = FMath::Max(--PrevQuantity, 0);

	if (NextQuantity > 0)
	{
		// #0. Quantity
		FString NewQuantityStr = FString::FromInt(NextQuantity);
		ItemQuantity->SetText(FText::FromString(NewQuantityStr));
		Quantity = NextQuantity;
	}
	else
	{
		// #0 Image
		ItemImage->SetBrushFromTexture(NullItemInfoStruct.ItemImage);
		// #1 Description
		ItemDescription->SetText(FText::FromString(NullItemInfoStruct.ItemDescription));
		// #2 Quantity
		ItemQuantity->SetText(FText::FromString(""));
		Quantity = NextQuantity;
		// #3 ItemInfoStruct
		ItemInfoStruct = NullItemInfoStruct;
		// #4 IsFilled?
		bFilled = false;

	}

	DropUseWindow->SetVisibility(ESlateVisibility::Hidden);

}

void UHUD_InventorySlot::RefreshItemSlot()
{

	if (bFilled == true && ItemQuantity > 0)
	{
		// #1 Image
		ItemImage->SetBrushFromTexture(ItemInfoStruct.ItemImage);
		// #2 Description
		ItemDescription->SetText(FText::FromString(ItemInfoStruct.ItemDescription));
		// #3 Quantity
		ItemQuantity->SetText(FText::FromString(FString::FromInt(Quantity)));
	}
	else
	{

		RemoveItemFromSlot();

	}

}


void UHUD_InventorySlot::UseItem()
{
	CheckFalse(bFilled);

	APlayerCharacter* OwnerPlayer =  Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	CheckNull(OwnerPlayer);

	switch (ItemInfoStruct.ItemType)
	{
	case EItemType::E_Potion:
	
		// #0 Use Potion
		UsePotionItem();
		// #1 Notify
		OnUseClicked.ExecuteIfBound(ItemInfoStruct);
		// #2 Remove
		RemoveItemFromSlot();

		break;
	case EItemType::E_Weapon:

		// #0 Use Weapon
		UseWeaponItem();
		// #1 Notify 		
		OnUseClicked.ExecuteIfBound(ItemInfoStruct);
		// #2 Remove Prev Item
		RemoveItemFromSlot();
		// #5 Notify
		IInventoryItemInterface* Interface = Cast<IInventoryItemInterface>(WeaponItem);
		CheckNull(Interface);
		OnAcquireReturnedItem.ExecuteIfBound(Interface->InteractWithInventory());
		// #6 Add New Item  
		AddNewItemToSlot(Interface->InteractWithInventory());

		break;
	};

}

void UHUD_InventorySlot::UsePotionItem()
{

	APlayerCharacter* OwnerPlayer = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	CheckNull(OwnerPlayer);

	AConsumableItem* ConsumableItem = GetWorld()->SpawnActor<AConsumableItem>(ItemInfoStruct.ItemClass);
	CheckNull(ConsumableItem);
	ConsumableItem->StartActivation(&OwnerPlayer);
	ConsumableItem->Destroy();

}

void UHUD_InventorySlot::UseWeaponItem()
{

	APlayerCharacter* OwnerPlayer = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
	if (OwnerPlayer)
	{
		// 현재 무기와 Inventory Slot의 무기 변경
		WeaponItem = GetWorld()->SpawnActor<AWeapon>(ItemInfoStruct.ItemClass);
		if (WeaponItem)
		{
			TArray<AWeapon*> Weapons = OwnerPlayer->GetWeaponComponent()->GetWeapons();
			for (int32 i = 0; i < Weapons.Num(); i++)
			{
				if (Weapons[i]->GetWeaponType() == WeaponItem->GetWeaponType())
				{
					OwnerPlayer->GetWeaponComponent()->SwapWeapon(Weapons[i]->GetWeaponType(), WeaponItem);
					WeaponItem = Weapons[i];
					break;
				}
			}
		}
	}

}


void UHUD_InventorySlot::DropItem()
{
	CheckFalse(bFilled);

	FVector ForwardVector = GetOwningPlayerPawn()->GetActorForwardVector();
	ForwardVector.Normalize();

	const FVector DropLocation = GetOwningPlayerPawn()->GetActorLocation() + ForwardVector * 200.f;
	const FRotator DropRotation = GetOwningPlayerPawn()->GetActorRotation();

	switch (ItemInfoStruct.ItemType)
	{
	case EItemType::E_Potion:
		
		// #0 전달
		OnDropClicked.ExecuteIfBound(ItemInfoStruct);
		// #1 Drop
		DropPotionItem(DropLocation - FVector(0.f, 0.f, 100.f), DropRotation);
		// #2 Remove
		RemoveItemFromSlot();

		break;

	case EItemType::E_Weapon:
		
		// #0 전달
		OnDropClicked.ExecuteIfBound(ItemInfoStruct);
		// #1 Drop
		DropWeaponItem(DropLocation, DropRotation);
		// #2 Remove
		RemoveItemFromSlot();

		break;

	};

}

void UHUD_InventorySlot::DropPotionItem(FVector InLocation, FRotator InRotation)
{

	AConsumableItem* DropConsumableItem;
	DropConsumableItem = GetWorld()->SpawnActor<AConsumableItem>(ItemInfoStruct.ItemClass, InLocation, InRotation);
	CheckNull(DropConsumableItem);

}

void UHUD_InventorySlot::DropWeaponItem(FVector InLocation, FRotator InRotation)
{

	AWeapon* DropWeaponItem;
	DropWeaponItem = GetWorld()->SpawnActor<AWeapon>(ItemInfoStruct.ItemClass, InLocation, InRotation);
	CheckNull(DropWeaponItem);
	DropWeaponItem->OnSelected();

}



/*	********************************************************************************************************
* 
*	[Remark] :	Drop/Use 창을 열고 닫는 과정에서 "AddToViewPort + RemoveFromParent" 동작을 "SetVisibility"로 대체
* 
*	목적:
*		Inventory에서 Item을 클릭하면, Drop/Use 창을 띄우고, 혹은 지우는 동작을 구현합니다.
* 
*	설명:
*		1. 화면에 그려내기위해 "AddToViewport", 화면에서 지우기위해 "Remove From Parent"를 활용했습니다.
*		2. 다만, 부모 Widget안에 존재하는 Child Widget일 경우, 에러를 발생시킵니다.
*		3. SetVisibility로 수정합니다.
* 
*	********************************************************************************************************/

void UHUD_InventorySlot::OnOffShowDropUseWindow()
{

	CheckFalse(bFilled);

	if (bDropUseWindowShownOnScreen == false)
	{
		DropUseWindowOverlay->AddChildToOverlay(DropUseWindow);
		DropUseWindow->SetVisibility(ESlateVisibility::Visible);

		bDropUseWindowShownOnScreen = true;
	}
	else
	{
		DropUseWindow->SetVisibility(ESlateVisibility::Hidden);

		bDropUseWindowShownOnScreen = false;
	}

}


void UHUD_InventorySlot::OnDescriptionWindow()
{
	CheckFalse(bFilled);

	// #1 Timer
	FTimerHandle TmpTimerHandler;
	float TmpRate = 1.0f;
	GetWorld()->GetTimerManager().SetTimer(TmpTimerHandler, FTimerDelegate::CreateLambda([&]() {
		ShowDescriptionText();
		}), TmpRate, false);

	bDescriptionWindowShownOnScreen = true;
}

void UHUD_InventorySlot::OffDescriptionWindow()
{
	CheckFalse(bFilled);
	CheckFalse(bDescriptionWindowShownOnScreen);

	ItemDescription->SetVisibility(ESlateVisibility::Hidden);

	bDescriptionWindowShownOnScreen = false;
}

void UHUD_InventorySlot::ShowDescriptionText()
{
	if (bDescriptionWindowShownOnScreen == true)
	{
		ItemDescription->SetVisibility(ESlateVisibility::Visible);
	}
}