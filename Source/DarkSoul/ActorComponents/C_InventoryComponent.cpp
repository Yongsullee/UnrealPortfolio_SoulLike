#include "C_InventoryComponent.h"
#include "Global/Global.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "GameObjects/PlayerCharacter.h"

#include"Interface/InventoryItemInterface.h"
#include "ConsumableItems/ConsumableItem.h"
#include "Weapon/Weapon.h"

#include "HUD/HUD_Inventory.h"
#include "HUD/HUD_InventorySlot.h"

UC_InventoryComponent::UC_InventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UC_InventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	CreateInventoryWidget();

	APlayerCharacter* OwnerPlayer = Cast<APlayerCharacter>(GetOwner());
	CheckTrue(!OwnerPlayer);

	OwnerPlayer->OnShowInventoryWidget.BindUFunction(this, "ShowInventoryWidgetOnScreen");
	OwnerPlayer->OnRemoveInventoryWidget.BindUFunction(this, "RemoveInventoryWidgetOnScreen");

	OwnerPlayer->OnAcquireItem.BindUFunction(this, "AcquireItem");

	for (const auto& InventoryWidget : InventoryWidgets)
	{
		InventoryWidget->OnInventoryHUDCloseButtonClicked.BindUFunction(this, "RemoveInventoryWidgetOnScreen");
		
		InventoryWidget->OnInventroyHUDSortComboBoxChanged.BindUFunction(this, "SwapInventoryWidget");
		
		InventoryWidget->OnUseClickedInInventorySlots.BindUFunction(this, "UseItemOccursInAnyInventoryWidget");
		InventoryWidget->OnDropClickedInInventorySlots.BindUFunction(this, "DropItemOccursInAnyInventoryWidget");

		InventoryWidget->OnInventorySlotAcquireReturnedItem.BindUFunction(this, "AcquireReturnedItem");
	}

}

bool UC_InventoryComponent::IsInventoryShownOnScreen()
{
	bool Check = false;
	for (const auto bShown : bShownOnScreen)
	{
		if (bShown == true)
		{
			Check = true;
			break;
		}
	}

	return Check;
}

/*	****************************************************************************************************************
* 
*	[Remark]	:	���� �ذ�, Inventory�� �ѳ��� "Acquire"Ű�� �Է��ؾ� Inventory�� ���� Item�� ���̴� ����
* 
*	����:
*		Inventory â�� �ѳ��� �ʰ�, ���� Inventory�� ��� �ִ� Item���� ������ ����.
* 
*	����:
*		#1 Inventory�� ���� �ʰ�, Ư�� Item�� �����ϸ�, Inventory�� ������ �ʴ� ������ �߻�..
*		#2 ���� �߰�, ShowInventoryWidgetOnScreen() �Լ����� �Ź� Inventory Widget�� �����ϰ� �־����ϴ�.
*		#3 ���� �ذ�, CreateWidget()�� BeginPlay()���� �ѹ��� ȣ���ϵ��� ShowInventoryWidgetOnScreen()���κ��� �и�
* 
*	****************************************************************************************************************/

void UC_InventoryComponent::CreateInventoryWidget()
{

	CheckNull(InventoryWidgetClass);

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	CheckNull(PlayerController);

	for (int32 i = 0; i < 3; i++)
	{
		UHUD_Inventory* TmpWidget = CreateWidget<UHUD_Inventory>(PlayerController, InventoryWidgetClass);
		TmpWidget->ItemCategoryType = static_cast<EItemType>(i);
		TmpWidget->AddToViewport();
		TmpWidget->SetVisibility(ESlateVisibility::Hidden);

		InventoryWidgets.Add(TmpWidget);
	}

}

/**********************************************************************
*
*	[Remark]: Redefinition Of Default Parameter Value
* 
*	����: �Լ� ���Ǻο��� Default�� ���� �ʿ� ���� 
* 
***********************************************************************/

void UC_InventoryComponent::ShowInventoryWidgetOnScreen(EItemType InType)
{

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	CheckNull(PlayerController);

	// #0 Sort
	if (InType != EItemType::E_All)
		InventoryWidgets[(int32)InType]->SortInventoryItems(InType);
	// #1 Visibility
	InventoryWidgets[(int32)InType]->SetVisibility(ESlateVisibility::Visible);
	// #2 Mouse Cursor
	PlayerController->bShowMouseCursor = true;
	// #3 Input Movde
	UWidgetBlueprintLibrary::SetInputMode_GameAndUI(PlayerController);
	// #4 IsShownOnScreen?
	bShownOnScreen[(int32)InType] = true;

}

void UC_InventoryComponent::RemoveInventoryWidgetOnScreen()
{

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	CheckNull(PlayerController);

	for (int32 i = 0; i < InventoryWidgets.Num(); i++)
	{
		if (bShownOnScreen[i] == true)
		{
			InventoryWidgets[i]->SetVisibility(ESlateVisibility::Hidden);
			PlayerController->bShowMouseCursor = false;
			UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController);

			bShownOnScreen[i] = false;
		}
	}

}

void UC_InventoryComponent::SwapInventoryWidget(EItemType InType)
{
	
	for (int32 i = 0; i < InventoryWidgets.Num(); i++)
	{
		if (bShownOnScreen[i] == true)
		{
			CheckTrue(i == (int32)InType);
			break;
		}
	}
	
	RemoveInventoryWidgetOnScreen();

	InventoryWidgets[(int32)InType]->SortInventoryItems(InType);

	InventoryWidgets[(int32)InType]->ChangeSortComboBoxOption((int32)InType);

	ShowInventoryWidgetOnScreen(InType);

}


/*	************************************************************************************************************
*
*	[Remark]	:	AActor::GetOverlappingActors() �� Ȱ���� Ư�� Ŭ������ Actor�� �����ɴϴ�.
*
*	����:
*		Consumable Item(eg, Potion)�� Player�� �浹�� ���� �ش� Item�� ������ �����ɴϴ�.
*	����:
*		#1	TSubclassOf<AConsumableItem> ConsumableItemClass �� �ܵ����� �����ϰ�,
*			GetOverlappingActors(OverlappingActors, ConsumbleItemClass) �� �ۼ��ϸ�, ����� �۵����� �ʽ��ϴ�!
*		#2	GetOverlappingActors(OverlappingActors, AConsumableItem::StaticClass()) �� �ۼ��ϸ�, ����� �۵��մϴ�.
*
*	************************************************************************************************************/

void UC_InventoryComponent::AcquireItem()
{
	bool Check = true;
	AActor* OwnerActor = GetOwner();
	CheckNull(OwnerActor);

	Check &= AcquireConsumableItems(&OwnerActor);

	if (Check == false) Check = AcquireWeaponItems(&OwnerActor);

}

bool UC_InventoryComponent::AcquireConsumableItems(AActor** OutActor)
{

	TSet<AActor*> OverlappingItems;
	(*OutActor)->GetOverlappingActors(OverlappingItems, AConsumableItem::StaticClass());

	if (OverlappingItems.Num() > 0)
	{
		for (const auto& OverlappingItem : OverlappingItems)
		{
			if (OverlappingItem->GetClass()->ImplementsInterface(UInventoryItemInterface::StaticClass()))
			{
				// #1 Interface
				IInventoryItemInterface* Interface = Cast<IInventoryItemInterface>(OverlappingItem);
				FInventoryItemInfo ItemInfoStruct = Interface->InteractWithInventory();

				// #2 "All" ī�װ��� �߰�
				if (InventoryWidgets[0]->AddNewItemToInventory(ItemInfoStruct) == true)
				{
					// #3 "Potion" ī�װ��� �߰�
					if (InventoryWidgets[(int32)EItemType::E_Potion]->AddNewItemToInventory(ItemInfoStruct) == true)
					{
						OverlappingItem->Destroy();
						return true;
					}
				}
			}
		}
	}

	return false;
}

bool UC_InventoryComponent::AcquireWeaponItems(AActor** OutActor)
{

	TSet <AActor*> OverlappingWeapons;
	(*OutActor)->GetOverlappingActors(OverlappingWeapons, AWeapon::StaticClass());

	if (OverlappingWeapons.Num() > 0)
	{
		for (const auto& OverlappingWeapon : OverlappingWeapons)
		{
			if (OverlappingWeapon->GetClass()->ImplementsInterface(UInventoryItemInterface::StaticClass()))
			{
				// #1 Interface
				IInventoryItemInterface* Interface = Cast<IInventoryItemInterface>(OverlappingWeapon);
				FInventoryItemInfo ItemInfoStruct = Interface->InteractWithInventory();

				// #2 "All" ī�װ��� �߰�
				if (InventoryWidgets[0]->AddNewItemToInventory(ItemInfoStruct) == true)
				{
					// #3 "Weapon" ī�װ��� �߰�
					if (InventoryWidgets[(int32)EItemType::E_Weapon]->AddNewItemToInventory(ItemInfoStruct) == true)
					{
						OverlappingWeapon->Destroy();
						return true;
					}
				}
			}
		}
	}

	return false;
}

/*	************************************************************************************************************************************
* 
*	[Remark] :	Ư�� Inventory Widget�� Ư�� Inventory Slot ���� "Use" ��ư�� ���� ���, ������ Inventory Widget�� ���� ������ �����մϴ�.
* 
*	����:
*		1. InventorySlot::OnUseClicked ���̳��� ��������Ʈ 
*		2. Inventory::OnUseClickedInInventorySlots ���̳��� ��������Ʈ
*		3. ���������, Inventory Component���� ������ Inventory Widget�� ���� ������ �����մϴ�.
* 
*	************************************************************************************************************************************/

void UC_InventoryComponent::UseItemOccursInAnyInventoryWidget(FInventoryItemInfo InUsedItemInfoStruct, EItemType InInventoryWidgetItemCategory)
{
	if (InInventoryWidgetItemCategory == EItemType::E_All)
	{

		switch (InUsedItemInfoStruct.ItemType)
		{
		case EItemType::E_Potion:
			for (int32 i = 0; i < InventoryWidgets[(int32)EItemType::E_Potion]->GetInventorySlots().Num(); i++)
			{
				if (InventoryWidgets[(int32)EItemType::E_Potion]->GetInventorySlots()[i]->GetInventoryItemInfoStruct().Name == InUsedItemInfoStruct.Name)
				{
					InventoryWidgets[(int32)EItemType::E_Potion]->GetInventorySlots()[i]->RemoveItemFromSlot();
				}
			}
			break;
		case EItemType::E_Weapon:
			// #1 Inevntory Slot�� ������ ����
			for (int32 i = 0; i < InventoryWidgets[(int32)EItemType::E_Weapon]->GetInventorySlots().Num(); i++)
			{
				if (InventoryWidgets[(int32)EItemType::E_Weapon]->GetInventorySlots()[i]->GetInventoryItemInfoStruct().Name == InUsedItemInfoStruct.Name)
				{
					InventoryWidgets[(int32)EItemType::E_Weapon]->GetInventorySlots()[i]->RemoveItemFromSlot();
				}
			}

			break;
		}
	}
	else
	{
		for (int32 i = 0; i < InventoryWidgets[(int32)EItemType::E_All]->GetInventorySlots().Num(); i++)
		{
			if (InventoryWidgets[(int32)EItemType::E_All]->GetInventorySlots()[i]->GetInventoryItemInfoStruct().Name == InUsedItemInfoStruct.Name)
			{
				InventoryWidgets[(int32)EItemType::E_All]->GetInventorySlots()[i]->RemoveItemFromSlot();
			}
		}
	}

}

/*	************************************************************************************************************************************
* 
*	[Remark] :	Ư�� Inventory Widget�� Ư�� Inventory Slot ���� "Use" ��ư�� ���� ���, ������ Inventory Widget�� ���� ������ �����մϴ�.
*
*	����:
*		1. InventorySlot::OnUseClicked ���̳��� ��������Ʈ (2 Params)
*		2. Inventory::OnUseClickedInInventorySlots ���̳��� ��������Ʈ (3 Params)
*		3. ���������, Inventory Component���� ������ Inventory Widget�� ���� ������ �����մϴ�.
* 
*	************************************************************************************************************************************/

void UC_InventoryComponent::DropItemOccursInAnyInventoryWidget(FInventoryItemInfo InDropItemInfoStruct, EItemType InInventoryWidgetItemCategory)
{
	if (InInventoryWidgetItemCategory == EItemType::E_All)
	{
		for (int32 i = 0; i < InventoryWidgets[(int32)InDropItemInfoStruct.ItemType]->GetInventorySlots().Num(); i++)
		{
			if (InventoryWidgets[(int32)InDropItemInfoStruct.ItemType]->GetInventorySlots()[i]->GetInventoryItemInfoStruct().Name == InDropItemInfoStruct.Name)
			{
				InventoryWidgets[(int32)InDropItemInfoStruct.ItemType]->GetInventorySlots()[i]->RemoveItemFromSlot();
			}
		}
	}
	else
	{
		for (int32 i = 0; i < InventoryWidgets[0]->GetInventorySlots().Num(); i++)
		{
			if (InventoryWidgets[0]->GetInventorySlots()[i]->GetInventoryItemInfoStruct().Name == InDropItemInfoStruct.Name)
			{
				InventoryWidgets[0]->GetInventorySlots()[i]->RemoveItemFromSlot();
			}
		}
	}

}

void UC_InventoryComponent::AcquireReturnedItem(FInventoryItemInfo InReturnedItemInfoStruct, EItemType InInventoryWidgetItemCategory)
{
	CheckFalse(InReturnedItemInfoStruct.ItemType == EItemType::E_Weapon);

	if (InInventoryWidgetItemCategory == EItemType::E_All)
	{
		InventoryWidgets[(int32)EItemType::E_Weapon]->AddNewItemToInventory(InReturnedItemInfoStruct);
	}
	else if (InInventoryWidgetItemCategory == EItemType::E_Weapon)
	{
		InventoryWidgets[(int32)EItemType::E_All]->AddNewItemToInventory(InReturnedItemInfoStruct);
	}

}
