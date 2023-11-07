#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "C_RewardComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKSOUL_API UC_RewardComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UC_RewardComponent();

protected:
	virtual void BeginPlay() override;
	
private:
	void LoadDataTableFromItemInfo();

public:
	UFUNCTION()
		void DropItem();

private:
	void DropPotion(const FVector& InDropLocation, const FRotator& InDropRotation);
	void DropWeapon(const FVector& InDropLocation, const FRotator& InDropRotation);

private:
	class UDataTable* DataTable_ItemInfo;
	FInventoryItemInfo DropItemInfo;

private:
	class AActor* OwnerActor = nullptr;

private:
	const float NormalRank_DropRate = 0.f;
	const float EpicRank_DropRate = 0.f;
	const float LegendaryRank_DropRate = 0.f;

};
