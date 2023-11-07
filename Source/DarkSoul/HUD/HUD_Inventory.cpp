#include "HUD_Inventory.h"
#include "Global/Global.h"

#include "HUD/HUD_InventorySlot.h"

#include "Components/Button.h"
#include "Components/ComboBoxString.h"

#include "Weapon/Weapon.h"

void UHUD_Inventory::NativeOnInitialized()
{

	Super::NativeOnInitialized();

	InventorySlots.Add(InventorySlot_1);
	InventorySlots.Add(InventorySlot_2);
	InventorySlots.Add(InventorySlot_3);
	InventorySlots.Add(InventorySlot_4);
	InventorySlots.Add(InventorySlot_5);
	InventorySlots.Add(InventorySlot_6);

	CloseButton->OnClicked.AddDynamic(this, &UHUD_Inventory::FinishInventory);
	SortComboBox->OnSelectionChanged.AddDynamic(this, &UHUD_Inventory::ChangeSortCategory);
	
	for (const auto& InventorySlot : InventorySlots)
	{
		InventorySlot->OnUseClicked.BindUFunction(this, "UseOccursInInventorySlots");
		InventorySlot->OnDropClicked.BindUFunction(this, "DropOccursInInventorySlots");
		InventorySlot->OnAcquireReturnedItem.BindUFunction(this, "AcquireReturnedItem");
	}

}

bool UHUD_Inventory::AddNewItemToInventory(FInventoryItemInfo InStruct)
{
	bool Check = false;

	Check = CheckIfExistingItemOnSlot(InStruct);

	if (Check == false) Check = CheckIfAnySlotIsEmpty(InStruct);

	return Check;

}

bool UHUD_Inventory::CheckIfExistingItemOnSlot(FInventoryItemInfo& InStruct)
{

	bool Res = false;
	for (int32 i = 0; i < InventorySlots.Num(); i++)
	{
		if (InventorySlots[i]->IsFilled() == true)
		{
			if (InStruct.Name == InventorySlots[i]->GetInventoryItemInfoStruct().Name
				&& InventorySlots[i]->GetInventoryItemInfoStruct().IsStackable == true)
			{
				// Add
				AddExistingItemToSlot(i);

				Res = true;
				break;
			}
		}
	}

	return Res;
}

bool UHUD_Inventory::CheckIfAnySlotIsEmpty(FInventoryItemInfo& InStruct)
{
	bool Res = false;
	for (int32 i = 0; i < InventorySlots.Num(); i++)
	{
		if (InventorySlots[i]->IsFilled() == false)
		{
			// Add
			AddNewItemToSlot(i, InStruct);

			Res = true;
			break;
		}
	}

	return Res;
}

void UHUD_Inventory::AddExistingItemToSlot(int32 InNum)
{

	CheckNull(InventorySlots[InNum]);
	CheckFalse(InventorySlots[InNum]->IsFilled());

	InventorySlots[InNum]->AddExistingItemToSlot();

}
void UHUD_Inventory::AddNewItemToSlot(int32 InNum, FInventoryItemInfo InStruct)
{
	CheckNull(InventorySlots[InNum]);
	CheckTrue(InventorySlots[InNum]->IsFilled());

	InventorySlots[InNum]->AddNewItemToSlot(InStruct);
}

void UHUD_Inventory::AcquireReturnedItem(FInventoryItemInfo InReturnedItemInfoStruct)
{
	OnInventorySlotAcquireReturnedItem.ExecuteIfBound(InReturnedItemInfoStruct, ItemCategoryType);
}

void UHUD_Inventory::ChangeSortCategory(FString InString, ESelectInfo::Type InType)
{

	if (InString == "All")
	{

		OnInventroyHUDSortComboBoxChanged.ExecuteIfBound(EItemType::E_All);

	}
	else if (InString == "Weapon")
	{

		OnInventroyHUDSortComboBoxChanged.ExecuteIfBound(EItemType::E_Weapon);

	}
	else if (InString == "Potion")
	{

		OnInventroyHUDSortComboBoxChanged.ExecuteIfBound(EItemType::E_Potion);

	}
}

void UHUD_Inventory::UseOccursInInventorySlots(FInventoryItemInfo InUsedItemInfoStruct)
{

	OnUseClickedInInventorySlots.ExecuteIfBound(InUsedItemInfoStruct, ItemCategoryType);

}

void UHUD_Inventory::DropOccursInInventorySlots(FInventoryItemInfo InDropItemInfoStruct)
{

	OnDropClickedInInventorySlots.ExecuteIfBound(InDropItemInfoStruct, ItemCategoryType);

}


void UHUD_Inventory::ChangeSortComboBoxOption(int32 InNum)
{
	switch (InNum)
	{
	case 0 :
		SortComboBox->SetSelectedOption("All");
		break;
	case 1:
		SortComboBox->SetSelectedOption("Potion");
		break;
	case 2:
		SortComboBox->SetSelectedOption("Weapon");
		break;
	}
}

void UHUD_Inventory::SortInventoryItems(EItemType InType)
{
	TArray<UHUD_InventorySlot*> TmpInventorySlots;
	TArray<TPair<FInventoryItemInfo, int32>> TmpPairs;
	switch (InType)
	{
	case EItemType::E_Potion:

		TmpInventorySlots = InventorySlots;
		Algo::Sort(TmpInventorySlots, [&](UHUD_InventorySlot* A, UHUD_InventorySlot* B)
			{
				return A->GetItemQuantity() > B->GetItemQuantity();
			});
		for (int32 i = 0; i < TmpInventorySlots.Num(); i++)
		{
			TPair<FInventoryItemInfo, int32> TmpPair;

			TmpPair.Key = TmpInventorySlots[i]->GetInventoryItemInfoStruct();
			TmpPair.Value = TmpInventorySlots[i]->GetItemQuantity();

			TmpPairs.Add(TmpPair);
		}

		for (int32 i = 0; i < InventorySlots.Num(); i++)
		{
			InventorySlots[i]->SetInventoryItemInfoStruct(TmpPairs[i].Key);
			InventorySlots[i]->SetItemQuantity(TmpPairs[i].Value);

			InventorySlots[i]->RefreshItemSlot();
		}

		break;

	case EItemType::E_Weapon:

		TmpInventorySlots = InventorySlots;
		Algo::Sort(TmpInventorySlots, [&](UHUD_InventorySlot* A, UHUD_InventorySlot* B)
			{
				// #1 EWeaponRank ¼ø¼­
				if (((int32)A->GetInventoryItemInfoStruct().WeaponType) == ((int32)B->GetInventoryItemInfoStruct().WeaponType))
					return ((int32)A->GetInventoryItemInfoStruct().WeaponRank) < ((int32)B->GetInventoryItemInfoStruct().WeaponRank);
				else
					return ((int32)A->GetInventoryItemInfoStruct().WeaponType) < ((int32)B->GetInventoryItemInfoStruct().WeaponType);
			}
		);
		for (int32 i = 0; i < TmpInventorySlots.Num(); i++)
		{
			TPair<FInventoryItemInfo, int32> TmpPair;
			TmpPair.Key = TmpInventorySlots[i]->GetInventoryItemInfoStruct();
			TmpPair.Value = TmpInventorySlots[i]->GetItemQuantity();

			TmpPairs.Add(TmpPair);
		}

		for (int32 i = 0; i < InventorySlots.Num(); i++)
		{
			InventorySlots[i]->SetInventoryItemInfoStruct(TmpPairs[i].Key);
			InventorySlots[i]->SetItemQuantity(TmpPairs[i].Value);

			InventorySlots[i]->RefreshItemSlot();
		}

		break;
	}
}



void UHUD_Inventory::FinishInventory()
{
	OnInventoryHUDCloseButtonClicked.ExecuteIfBound();
}