#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "C_TargetComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKSOUL_API UC_TargetComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UC_TargetComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION()
		void LockOnTarget();
	UFUNCTION()
		void CycleClockwise();
	UFUNCTION()
		void CycleAntiClockwise();

private:
	void FindNearbyActors();
	void FindTargetEnemy();

	void FocusOnTarget(float DeltaTime);

private:
	class AGameObject* OwnerObject;

private:

	UPROPERTY(VisibleAnywhere, Category = "Targeting")
		class AGameObject* TargetEnemy = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Targeting")
		TArray<class AGameObject*> NearbyEnemies;

	UPROPERTY(VisibleAnywhere, Category = "Targeting")
		TMap<class AGameObject*, float> MBoundEnemies;

	UPROPERTY(EditDefaultsOnly, Category = "Targeting")
		float EnemyDetectionMaxRadius = 1000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Targeting")
		float InteropSpeed = 20.f;

	UPROPERTY(VisibleAnywhere, Category = "Targeting")
		bool TargetingLockOn = false;

public:
	FORCEINLINE bool GetTargetingLockOn() { return TargetingLockOn; }
};
