#include "C_RewardComponent.h"
#include "Global/Global.h"

#include "ConsumableItems/ConsumableItem.h"
#include "Weapon/Weapon.h"

#include "ActorComponents/C_HealthComponent.h"
#include "Interface/OwnerWeaponInterface.h"


UC_RewardComponent::UC_RewardComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	CHelpers::GetAsset(&DataTable_ItemInfo, "DataTable'/Game/DataTables/Items/DataTable_ItemsInfo.DataTable_ItemsInfo'");

}


void UC_RewardComponent::BeginPlay()
{

	Super::BeginPlay();
	// #0. Item Info From Data Table
	LoadDataTableFromItemInfo();

	OwnerActor = GetOwner();
	UC_HealthComponent* HealthComp = OwnerActor->FindComponentByClass<UC_HealthComponent>();
	CheckTrue(!OwnerActor || !HealthComp);

	HealthComp->OnDropRewardItem.BindUFunction(this, "DropItem");

}

void UC_RewardComponent::LoadDataTableFromItemInfo()
{

	CheckNull(DataTable_ItemInfo);

	TArray<FName>RowNames = DataTable_ItemInfo->GetRowNames();

	const FString ContextString(TEXT("Drop Item Info"));

	float RandomIndex = FMath::RandRange(0, RowNames.Num() - 1);
	FInventoryItemInfo* RowStruct = DataTable_ItemInfo->FindRow<FInventoryItemInfo>(RowNames[RandomIndex], ContextString, true);

	DropItemInfo = *RowStruct;

}

void UC_RewardComponent::DropItem()
{

	IOwnerWeaponInterface* Interface = Cast<IOwnerWeaponInterface>(OwnerActor);

	CheckTrue(!OwnerActor || !Interface || Interface->GetOwnerStateType() != EStateType::E_Dead);

	FVector ForwardVector = OwnerActor->GetActorForwardVector();
	ForwardVector.Normalize();

	const FVector DropLocation = OwnerActor->GetActorLocation() + ForwardVector * 200.f;
	const FRotator DropRotation = OwnerActor->GetActorRotation();

	float RandNum = FMath::RandRange(0.f, 1.0f);

	switch (DropItemInfo.ItemType)
	{
	case EItemType::E_Potion:
		DropPotion(DropLocation - FVector(0.f, 0.f, 100.f), DropRotation);
		break;
	case EItemType::E_Weapon:
		if (DropItemInfo.WeaponRank == EWeaponRank::E_Normal)
		{
			if (RandNum >= NormalRank_DropRate) DropWeapon(DropLocation, DropRotation);
		}
		else if (DropItemInfo.WeaponRank == EWeaponRank::E_Epic)
		{
			if (RandNum >= EpicRank_DropRate) DropWeapon(DropLocation, DropRotation);
		}
		else if (DropItemInfo.WeaponRank == EWeaponRank::E_Legend)
		{
			if (RandNum >= LegendaryRank_DropRate) DropWeapon(DropLocation, DropRotation);
		}
		break;
	case EItemType::E_Max:
		break;
	}

}

void UC_RewardComponent::DropPotion(const FVector& InDropLocation, const FRotator& InDropRotation)
{

	AConsumableItem* DropConsumableItem;
	DropConsumableItem = GetWorld()->SpawnActor<AConsumableItem>(DropItemInfo.ItemClass, InDropLocation, InDropRotation);

}

void UC_RewardComponent::DropWeapon(const FVector& InDropLocation, const FRotator& InDropRotation)
{

	AWeapon* DropWeaponItem;
	DropWeaponItem = GetWorld()->SpawnActor<AWeapon>(DropItemInfo.ItemClass, InDropLocation, InDropRotation);

	CheckTrue(!DropWeaponItem);
	DropWeaponItem->OnSelected();

}
