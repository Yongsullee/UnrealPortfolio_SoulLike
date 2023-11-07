#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Global/Global.h"

#include "AIC_EnemyBase.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FChangeAgentAIState, EAIState, InType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAgentStartAttack);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAgentRunAICombatPhase, EAICombatPhase, InEnum);

UCLASS()
class DARKSOUL_API AAIC_EnemyBase : public AAIController
{
	GENERATED_BODY()
	
public:
	AAIC_EnemyBase();

//	******************************************************************************************
				// Method
//	******************************************************************************************

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void UpdateControlRotation(float DeltaTime, bool bUpdatePawn = true);

protected:
	UFUNCTION()
		virtual void OnPossess(class APawn* InPawn) override;

public:
	UFUNCTION()
		virtual void OnPerception(AActor* Actor, FAIStimulus Stimulus);

public:
	virtual void Attack();
	virtual void RunAICombatPhase(EAICombatPhase InEnum);
	virtual void ChangeAgentAIState(EAIState InStateType);

public:
	FAgentStartAttack OnAgentStartAttack;
	FAgentRunAICombatPhase OnAgentRunAICombatPhase;
	FChangeAgentAIState OnChangeAgentAIState;

//	******************************************************************************************
			// Data Member
//	******************************************************************************************

protected:
	UPROPERTY(Transient)
		class UBehaviorTreeComponent* AIBehaviorTree;
	UPROPERTY(Transient)
		class UBlackboardComponent* BBComponent;

	class AAIManager* AIManager = nullptr;

private:
	UPROPERTY(VisibleAnywhere)
		UAIPerceptionComponent* AIPerceptionComponent;

	class UAISenseConfig_Sight* Sight;
	class UAISenseConfig_Hearing* Hearing;

private:
	UPROPERTY(VisibleAnywhere)
		FRotator TargetRotation = FRotator::ZeroRotator;

	UPROPERTY(EditDefaultsOnly)
		float SmootRotationSpeed = 540.f;

protected:
	// [23-05-03] Remark : Skill Range Ãß°¡
	TTuple<float, float> AttackRange;
	TTuple<float, float> SkillRange;

//	******************************************************************************************
			// Getter & Setter
//	******************************************************************************************

public:
	FORCEINLINE class AAIManager* GetAIManager() { return AIManager; }
	FORCEINLINE TTuple<float, float> GetAttackRange() { return AttackRange; }
	FORCEINLINE TTuple<float, float> GetSkillRange() { return SkillRange; }

	FORCEINLINE void SetAIManager(class AAIManager* InManager) { AIManager = InManager; }
	FORCEINLINE void SetTargetRotation(FRotator InRotator) { TargetRotation = InRotator; }
	FORCEINLINE void SetAttackRange(TTuple<float, float> InTuple) { AttackRange = InTuple; }


};
