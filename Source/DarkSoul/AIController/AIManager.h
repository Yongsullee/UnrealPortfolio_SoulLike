#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Global/Global.h"

#include "AIManager.generated.h"

UCLASS()	
class DARKSOUL_API AAIManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AAIManager();

protected:
	virtual void BeginPlay() override;

public:
	void CreateAgentList(TArray<class AEnemyBase*>& OutArray, class AEnemyBase* InBossEnemy = nullptr);


public:
	UFUNCTION()
		void NotifyAIState(EAIState InDesireState, class AActor* InTargetObject = nullptr, FVector InTargetLocation = FVector::ZeroVector);

	UFUNCTION()
		void RemoveAgent(class AAIC_EnemyBase* ControllerToRemove);

private:
	UPROPERTY(VisibleAnywhere)
		TArray<class AAIC_EnemyBase*> Agents;

protected:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "AI")
		EFaction Faction;

	FTimerHandle CombatTimer;	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "AI")
		float MaxStimulusTime_Combat = 10.f;
	
	FTimerHandle SearchTimer;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "AI")
		float MaxStimulusTime_Search = 10.f;

	float TimeStamp = 0.f;

public:
	FORCEINLINE void SetFaction(EFaction InType) { Faction = InType; }

	FORCEINLINE EFaction GetFaction() { return Faction; }
	FORCEINLINE TArray<class AAIC_EnemyBase*> GetAllAgents() { return Agents; }


};
