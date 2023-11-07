#include "AIManager.h"
#include "Global/Global.h"

#include "GameObjects/EnemyBase.h"
#include "AIController/AIC_CastleKnight.h"
#include "AIController/AIC_CastleArchery.h"
#include "BehaviorTree/BlackboardComponent.h"

AAIManager::AAIManager()
{

	PrimaryActorTick.bCanEverTick = false;

}

void AAIManager::BeginPlay()
{

	Super::BeginPlay();

}

void AAIManager::CreateAgentList(TArray<class AEnemyBase*>& OutArray, class AEnemyBase* InBossEnemy)
{

	AAIC_EnemyBase* Controller;

	/*	************************************************************************************************************
	*	Remark [22-04-01]
	*	설명: Boss Enemy의 경우 Enemy의 AIContoller와 관계없는 특수한 행동을 정의할 수 있으므로, 수정이 필요할 수도 있음..
	*	************************************************************************************************************/

	if (!!InBossEnemy)
	{
		Controller = Cast<AAIC_EnemyBase>(InBossEnemy->GetController());
		if (!!Controller && InBossEnemy->GetFaction() == Faction)
		{
			Controller->SetAIManager(this);
			Agents.Add(Controller);
		}
	}

	if (OutArray.Num() > 0)
	{
		for (uint8 i = 0; i < OutArray.Num(); i++)
		{
			if (!!OutArray[i])
			{
				Controller = Cast<AAIC_EnemyBase>(OutArray[i]->GetController());
				if (!!Controller && OutArray[i]->GetFaction() == Faction)
				{
					Controller->SetAIManager(this);
					Agents.Add(Controller);
				}
			}
		}
	}

}

void AAIManager::NotifyAIState(EAIState InDesireState, AActor* InTargetObject, FVector InTargetLocation)
{

	for (auto& Agent : Agents)
	{
		if (Agent)
		{
			Agent->GetBlackboardComponent()->SetValueAsEnum("AIState", (uint8)InDesireState);
			Agent->GetBlackboardComponent()->SetValueAsVector("MoveToLocation", InTargetLocation);
			Agent->GetBlackboardComponent()->SetValueAsObject("TargetActor", InTargetObject);
		}
	}

}

void AAIManager::RemoveAgent(AAIC_EnemyBase* ControllerToRemove)
{

	int32 Index = Agents.Find(ControllerToRemove);

	if(Agents.IsValidIndex(Index)) Agents.RemoveSingle(ControllerToRemove);

}
