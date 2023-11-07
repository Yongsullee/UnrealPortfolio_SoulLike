#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Grid.generated.h"

UCLASS()
class DARKSOUL_API AGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	AGrid();

protected:
	virtual void BeginPlay() override;

protected:
	virtual void Tick(float DeltaTime) override;

private:
	void GetSmartObjectsOnGrid();
	void GetEnemiesOnGrid();
	void SpawnAIManagerOnGrid();

private:
	UFUNCTION()
		void EnemyIsDead(class AActor* InDamageCauser, bool IsBoss, uint8 InIndex);
	UFUNCTION()
		void MoveToReadyToRegenLocation(class AActor* InDamageCauser, bool IsAllEnemyDead, uint8 InIndex = -1);

private:
	void StartRegenCoolTime(uint8 InIndex);
	void CountRegenCoolTime();
	void RegenEnemy(uint8 InIndex);

public:
	UFUNCTION()
		void HandleLevelSeqStarted();
	UFUNCTION()
		void HandleLevelSeqFinished();

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
		class UBoxComponent* Box;

private:
	TArray<class AEnemyBase*> EnemyGroupOnGrid;

	// #2. Boss Enemy
	class AEnemyBase* BossEnemy;

	// #3. Smart Object(Patrol Path)
	TArray<class ASmartObject*> SmartObjectsOnGrid;

	// #4. AI Manager(OverrideAIState)
	const uint8 Max_AIManagers = 3;
	TArray<class AAIManager*> AIManagersOnGrid;

private:
	FVector ReadyLocation;
	const float MoveToReadyToRegenLocationRate = 2.f;

private:
	const float CoolTime = 100.f;
	TArray<bool> EnemyReadyToRespawn;
	TArray<float> RespawnCoolTimer;

protected:
	UPROPERTY(EditAnywhere, Category = "Level Sequence Handling")
		bool DeactivateGridWhileLevelSeqPlay = false;

public:
	FORCEINLINE TArray<class AEnemyBase*> GetEnemyGroupOnGrid() { return EnemyGroupOnGrid; }
	FORCEINLINE AEnemyBase* GetBossEnemyOnGrid() { return BossEnemy; }

};
