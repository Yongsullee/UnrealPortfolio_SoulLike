#include "DarkSoulGameModeBase.h"
#include "GameFramework/Actor.h"
#include "Global/Global.h"

#include "GameObjects/PlayerCharacter.h"

#include "GameObjects/EnemyBase.h"
#include "GameMode/TP_EnemySpawnLocation.h"

#include "Weapon/Weapon.h"
#include "ActorComponents/C_WeaponComponent.h"
#include "GameMode/TP_WeaponSpawnLocation.h"

#include "ConsumableItems/ConsumableItem.h"
#include "GameMode/TP_ConsumableItemSpawnLocation.h"

#include "Space/Grid.h"

#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "ActorComponents/C_AudioComponent.h"

ADarkSoulGameModeBase::ADarkSoulGameModeBase()
{

	// Default Pawn Class
	TSubclassOf<APlayerCharacter> playerClass;
	CHelpers::GetClass<APlayerCharacter>(&playerClass, "Blueprint'/Game/Blueprints/PlayerCharacter/BP_PlayerCharacter_New'");
	CheckNull(playerClass);

	DefaultPawnClass = playerClass;

}

void ADarkSoulGameModeBase::BeginPlay()
{
	
	// #0. Spawn Location
	if (bSpawnPointsHasLoaded == false)
	{
		SpawnEnemyAtTargetPoints();
		SpawnWeaponAtTargetPoints();
		SpawnConsumableItemAtTargetPoints();

		bSpawnPointsHasLoaded = true;
	}



}


void ADarkSoulGameModeBase::SpawnEnemyAtTargetPoints()
{
	TArray<AActor*> TempArr;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATP_EnemySpawnLocation::StaticClass(), TempArr);

	if (TempArr.IsValidIndex(0))
	{
		for (const auto TargetPoint : TempArr)
		{
			ATP_EnemySpawnLocation* SpawnLocation = Cast<ATP_EnemySpawnLocation>(TargetPoint);
			CheckNull(SpawnLocation);

			AEnemyBase* Enemy = GetWorld()->SpawnActor<AEnemyBase>(SpawnLocation->GetEnemyClass(), SpawnLocation->GetActorLocation(), FRotator::ZeroRotator);
			CheckNull(Enemy);

		}
	}
}

void ADarkSoulGameModeBase::SpawnWeaponAtTargetPoints()
{
	TArray<AActor*> TempArr;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATP_WeaponSpawnLocation::StaticClass(), TempArr);

	if (TempArr.IsValidIndex(0))
	{
		for (const auto TargetPoint : TempArr)
		{
			ATP_WeaponSpawnLocation* SpawnLocation = Cast<ATP_WeaponSpawnLocation>(TargetPoint);
			CheckNull(SpawnLocation);

			AWeapon* Weapon = GetWorld()->SpawnActor<AWeapon>(SpawnLocation->GetWeaponClass(), SpawnLocation->GetActorLocation(), FRotator::ZeroRotator);
			CheckNull(Weapon);
		}
	}
}

void ADarkSoulGameModeBase::SpawnConsumableItemAtTargetPoints()
{
	TArray<AActor*> TempArr;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATP_ConsumableItemSpawnLocation::StaticClass(), TempArr);

	if (TempArr.IsValidIndex(0))
	{
		for (const auto TargetPoint : TempArr)
		{
			ATP_ConsumableItemSpawnLocation* SpawnLocation = Cast<ATP_ConsumableItemSpawnLocation>(TargetPoint);
			CheckNull(SpawnLocation);

			AConsumableItem* Item = GetWorld()->SpawnActor<AConsumableItem>(SpawnLocation->GetConsumableItemClass(), SpawnLocation->GetActorLocation(), FRotator::ZeroRotator);
			CheckNull(Item);
		}
	}
}
