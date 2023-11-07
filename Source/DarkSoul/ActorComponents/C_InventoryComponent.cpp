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
*	[Remark]	:	문제 해결, Inventory를 켜놓고 "Acquire"키를 입력해야 Inventory에 현재 Item이 보이는 문제
* 
*	목적:
*		Inventory 창을 켜놓지 않고도, 현재 Inventory에 들어 있는 Item들의 정보를 유지.
* 
*	설명:
*		#1 Inventory를 켜지 않고, 특정 Item을 습득하면, Inventory에 보이지 않는 문제가 발생..
*		#2 문제 발견, ShowInventoryWidgetOnScreen() 함수에서 매번 Inventory Widget을 생성하고 있었습니다.
*		#3 문제 해결, CreateWidget()을 BeginPlay()에서 한번만 호출하도록 ShowInventoryWidgetOnScreen()으로부터 분리
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
*	설명: 함수 정의부에서 Default값 적을 필요 없음 
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
*	[Remark]	:	AActor::GetOverlappingActors() 를 활용해 특정 클래스의 Actor를 가져옵니다.
*
*	목적:
*		Consumable Item(eg, Potion)과 Player의 충돌을 통해 해당 Item의 정보를 가져옵니다.
*	설명:
*		#1	TSubclassOf<AConsumableItem> ConsumableItemClass 를 단독으로 선언하고,
*			GetOverlappingActors(OverlappingActors, ConsumbleItemClass) 로 작성하면, 제대로 작동하지 않습니다!
*		#2	GetOverlappingActors(OverlappingActors, AConsumableItem::StaticClass()) 로 작성하면, 제대로 작동합니다.
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

				// #2 "All" 카테고리에 추가
				if (InventoryWidgets[0]->AddNewItemToInventory(ItemInfoStruct) == true)
				{
					// #3 "Potion" 카테고리에 추가
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

				// #2 "All" 카테고리에 추가
				if (InventoryWidgets[0]->AddNewItemToInventory(ItemInfoStruct) == true)
				{
					// #3 "Weapon" 카테고리에 추가
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
*	[Remark] :	특정 Inventory Widget의 특정 Inventory Slot 에서 "Use" 버튼이 눌릴 경우, 나머지 Inventory Widget에 변경 사항을 적용합니다.
* 
*	목적:
*		1. InventorySlot::OnUseClicked 다이내믹 델리게이트 
*		2. Inventory::OnUseClickedInInventorySlots 다이내믹 델리게이트
*		3. 결과적으로, Inventory Component에서 나머지 Inventory Widget에 변경 사항을 적용합니다.
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
			// #1 Inevntory Slot의 아이템 제거
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
*	[Remark] :	특정 Inventory Widget의 특정 Inventory Slot 에서 "Use" 버튼이 눌릴 경우, 나머지 Inventory Widget에 변경 사항을 적용합니다.
*
*	목적:
*		1. InventorySlot::OnUseClicked 다이내믹 델리게이트 (2 Params)
*		2. Inventory::OnUseClickedInInventorySlots 다이내믹 델리게이트 (3 Params)
*		3. 결과적으로, Inventory Component에서 나머지 Inventory Widget에 변경 사항을 적용합니다.
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
