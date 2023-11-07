#include "AIC_EnemyBase.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "AIController/AIManager.h"
#include "SmartObject.h"

#include "Runtime/AIModule/Classes/Perception/AISenseConfig_Sight.h"
#include "Runtime/AIModule/Classes/Perception/AISenseConfig_Hearing.h"

#include "GameObjects/GameObject.h"


AAIC_EnemyBase::AAIC_EnemyBase()
{
	
	PrimaryActorTick.bCanEverTick = true;

	AIBehaviorTree = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behavior Tree"));
	BBComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception"));
	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AI Sight Config"));
	Hearing = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("AI Hearing Config"));

	Sight->SightRadius = 2000.f;
	Sight->LoseSightRadius = Sight->SightRadius + 500.f;
	Sight->PeripheralVisionAngleDegrees = 90.f;
	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectFriendlies = true;
	Sight->DetectionByAffiliation.bDetectNeutrals = true;

	Hearing->HearingRange = 2000.f;
	Hearing->DetectionByAffiliation.bDetectEnemies = true;
	Hearing->DetectionByAffiliation.bDetectNeutrals = true;
	Hearing->DetectionByAffiliation.bDetectFriendlies = true;

	AIPerceptionComponent->ConfigureSense(*Sight);
	AIPerceptionComponent->ConfigureSense(*Hearing);
	AIPerceptionComponent->SetDominantSense(Sight->GetSenseImplementation());
	AIPerceptionComponent->bAutoActivate = true;

}

void AAIC_EnemyBase::BeginPlay()
{

	Super::BeginPlay();

}

void AAIC_EnemyBase::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);

	// #0. RInterp
	UpdateControlRotation(DeltaTime);

}

void AAIC_EnemyBase::UpdateControlRotation(float DeltaTime, bool bUpdatePawn)
{

	Super::UpdateControlRotation(DeltaTime, false);

	APawn* const AgentPawn = GetPawn();
	CheckNull(AgentPawn);
	const FRotator CurrentPawnRotation = AgentPawn->GetActorRotation();

	FRotator Rotation = UKismetMathLibrary::RInterpTo_Constant(AgentPawn->GetActorRotation(), ControlRotation, DeltaTime, SmootRotationSpeed);
	if (CurrentPawnRotation.Equals(Rotation, 1e-3f) == false)
	{
		AgentPawn->FaceRotation(Rotation, DeltaTime);
	}

}

void AAIC_EnemyBase::OnPossess(class APawn* InPawn)
{

	Super::OnPossess(InPawn);

	AGameObject* OwningObject = Cast<AGameObject>(InPawn);

	CheckTrue(!OwningObject || !OwningObject->GetBehaviorTreeAsset());

	// #1. BBComponent + BT
	if (!!BBComponent && !!AIBehaviorTree)
	{
		BBComponent->InitializeBlackboard(*OwningObject->GetBehaviorTreeAsset()->BlackboardAsset);
		AIBehaviorTree->StartTree(*OwningObject->GetBehaviorTreeAsset());
	}
	// #2. Delegate
	if (!!AIPerceptionComponent)
	{
		AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AAIC_EnemyBase::OnPerception);
	}

}

void AAIC_EnemyBase::OnPerception(AActor* Actor, FAIStimulus Stimulus)
{

	AGameObject* OwningObject = Cast<AGameObject>(GetPawn());
	AGameObject* SensedObject = Cast<AGameObject>(Actor);

	CheckTrue(!OwningObject || !SensedObject || !OwningObject->IsHostile(SensedObject));

	if (Stimulus.WasSuccessfullySensed())
	{
		BBComponent->SetValueAsBool("Contact", Stimulus.WasSuccessfullySensed());
		BBComponent->SetValueAsObject("TargetActor", Actor);
		BBComponent->SetValueAsVector("MoveToLocation", Stimulus.StimulusLocation);
	}

}


void AAIC_EnemyBase::Attack()
{

	OnAgentStartAttack.Broadcast();

}

void AAIC_EnemyBase::RunAICombatPhase(EAICombatPhase InEnum)
{

	OnAgentRunAICombatPhase.Broadcast(InEnum);

}

void AAIC_EnemyBase::ChangeAgentAIState(EAIState InStateType)
{

	OnChangeAgentAIState.ExecuteIfBound(InStateType);

}


