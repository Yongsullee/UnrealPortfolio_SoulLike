#include "Grid.h"
#include "Global/Global.h"

#include "Components/BoxComponent.h"

#include "GameObjects/EnemyBase.h"
#include "GameObjects/Enemy.h"
#include "GameObjects/BossEnemy.h"
#include "AIController/SmartObject.h"
#include "AIController/AIManager.h"
#include "AIController.h"
#include "BrainComponent.h"

#include "ActorComponents/C_WeaponComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "LevelSequenceActor.h"


AGrid::AGrid()
{

	PrimaryActorTick.bCanEverTick = true;

	// #1. Component
	CHelpers::CreateComponent<UBoxComponent>(this, &Box, "Box Component");

	Box->SetupAttachment(RootComponent);
	Box->SetBoxExtent(FVector(1000.f, 1000.f, 100.f));
	Box->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Box->SetHiddenInGame(true);
	Box->SetVisibility(false);

	// #3. AIManagers Size : Enemy, Neutral, Friendly
	AIManagersOnGrid.Init(nullptr, Max_AIManagers);
	// [23-09-09] : Adjust Ready Location 
	ReadyLocation = Box->Bounds.GetBox().GetCenter() + FVector(0.f, 0.f, 2000.f);

}


void AGrid::BeginPlay()
{

	Super::BeginPlay();

	// #0. Smart Objects
	GetSmartObjectsOnGrid();
	// #1. Spawn Enemy
	GetEnemiesOnGrid();
	// #3. Spawn AIManagers
	SpawnAIManagerOnGrid();

	// #4. Respawn
	EnemyReadyToRespawn.Init(false, EnemyGroupOnGrid.Num());
	RespawnCoolTimer.Init(0.f, EnemyGroupOnGrid.Num());

	// #5. Delegates
	if(BossEnemy) BossEnemy->OnEnemyIsDead.BindUFunction(this, "EnemyIsDead");

	if (EnemyGroupOnGrid.Num() > 0)
	{
		for (auto Enemy : EnemyGroupOnGrid)
		{
			if (Enemy)
			{
				Enemy->OnEnemyIsDead.BindUFunction(this, "EnemyIsDead");
			}
		}
	}

	// [23-09-08] : Level Sequence And Cinematic
	if (DeactivateGridWhileLevelSeqPlay)
	{
		HandleLevelSeqStarted();

		TArray<AActor*> AllActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALevelSequenceActor::StaticClass(), AllActors);
		for (auto Actor : AllActors)
		{
			if (Actor)
			{
				ALevelSequenceActor* LevelSeq = Cast<ALevelSequenceActor>(Actor);
				if (LevelSeq && LevelSeq->GetSequencePlayer())
				{
					LevelSeq->GetSequencePlayer()->OnFinished.AddDynamic(this, &AGrid::HandleLevelSeqFinished);
				}
			}
		}
	}

}

void AGrid::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);

	// #1. Count Regen Cool Time
	CountRegenCoolTime();

}

void AGrid::GetSmartObjectsOnGrid()
{

	TArray<AActor*> AllSmartObjects;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASmartObject::StaticClass(), AllSmartObjects);
	for (auto SmartObject : AllSmartObjects)
	{
		if (Box->Bounds.GetBox().IsInsideOrOn(SmartObject->GetActorLocation()))
		{
			SmartObjectsOnGrid.Add(Cast<ASmartObject>(SmartObject));
		}
	}

}

void AGrid::GetEnemiesOnGrid()
{

	TArray<AActor*> AllEnemyBaseActorsOnGrid;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), AllEnemyBaseActorsOnGrid);
	uint8 cnt = 0;
	for (auto EnemyBaseActorOnGrid : AllEnemyBaseActorsOnGrid)
	{
		if (Box->Bounds.GetBox().IsInsideOrOn(EnemyBaseActorOnGrid->GetActorLocation()))
		{
			AEnemy* Enemy = Cast<AEnemy>(EnemyBaseActorOnGrid);
			if (Enemy && SmartObjectsOnGrid.Num() > 0)
			{
				uint8 RandSmartObjectIdx = -1;
				RandSmartObjectIdx = FMath::RandRange(0, SmartObjectsOnGrid.Num() - 1);

				Enemy->SetIndex(cnt++);
				Enemy->SetSmartObject(SmartObjectsOnGrid[RandSmartObjectIdx]);

				EnemyGroupOnGrid.Add(Enemy);
			}
		}
	}

	TArray<AActor*> AllBossEnemyActorsOnGrid;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABossEnemy::StaticClass(), AllBossEnemyActorsOnGrid);
	for (auto BossEnemyActorOnGrid : AllBossEnemyActorsOnGrid)
	{
		BossEnemy = Cast<ABossEnemy>(BossEnemyActorOnGrid);
		if (BossEnemy && SmartObjectsOnGrid.Num() > 0)
		{
			uint8 RandSmartObjectIdx = -1;
			RandSmartObjectIdx = FMath::RandRange(0, SmartObjectsOnGrid.Num() - 1);
			
			BossEnemy->SetSmartObject(SmartObjectsOnGrid[RandSmartObjectIdx]);
		}
	}

}

void AGrid::SpawnAIManagerOnGrid()
{

	// #0. Enemy AI Manager
	AAIManager* EnemyAIManager = GetWorld()->SpawnActor<AAIManager>(Box->Bounds.GetBox().GetCenter(), FRotator::ZeroRotator);
	EnemyAIManager->SetFaction(EFaction::E_Enemy);
	EnemyAIManager->SetActorHiddenInGame(true);
	if (BossEnemy)
	{
		EnemyAIManager->CreateAgentList(EnemyGroupOnGrid, BossEnemy);
	}
	else
	{
		EnemyAIManager->CreateAgentList(EnemyGroupOnGrid);
	}
	AIManagersOnGrid[0%Max_AIManagers] = (EnemyAIManager);
	
	// #1. Neutral AI Manager
	AAIManager* NeutralAIManager = GetWorld()->SpawnActor<AAIManager>(Box->Bounds.GetBox().GetCenter(), FRotator::ZeroRotator);
	NeutralAIManager->SetFaction(EFaction::E_Neutral);
	NeutralAIManager->SetActorHiddenInGame(true);
	if (BossEnemy)
	{
		NeutralAIManager->CreateAgentList(EnemyGroupOnGrid, BossEnemy);
	}
	else
	{
		NeutralAIManager->CreateAgentList(EnemyGroupOnGrid);
	}
	AIManagersOnGrid[1%Max_AIManagers] = (NeutralAIManager);

	// #2. Friendly AI Manager
	AAIManager* FriendlyAIManager = GetWorld()->SpawnActor<AAIManager>(Box->Bounds.GetBox().GetCenter(), FRotator::ZeroRotator);
	FriendlyAIManager->SetFaction(EFaction::E_Friendly);
	FriendlyAIManager->SetActorHiddenInGame(true);
	if (BossEnemy)
	{
		FriendlyAIManager->CreateAgentList(EnemyGroupOnGrid, BossEnemy);
	}
	else
	{
		FriendlyAIManager->CreateAgentList(EnemyGroupOnGrid);
	}
	AIManagersOnGrid[2%Max_AIManagers] = (FriendlyAIManager);

}

void AGrid::EnemyIsDead(AActor* InDamageCauser, bool IsBoss, uint8 InIndex)
{

	if (InDamageCauser)
	{
		if (IsBoss)
		{
			MoveToReadyToRegenLocation(InDamageCauser, true);
		}
		else
		{
			// #0. Move To Ready Location
			MoveToReadyToRegenLocation(InDamageCauser, false, InIndex);
			
			// TEST : Respawn Temporarily Function Off
			//StartRegenCoolTime(InIndex);
		}
	}

}

/*
*	[23-09-08]
* 
*	문제	:	몇몇 Enemy가 Ready To Regen Location으로 이동하지 않는 문제 발견
* 
*	해결 :	FTimerHandle을 데이터 멤버에서 지역 변수로 선언하는 것으로 변경합니다.
*	
*/
void AGrid::MoveToReadyToRegenLocation(AActor* InDamageCauser, bool IsAllEnemyDead, uint8 InIndex)
{

	if (InDamageCauser && InIndex > EnemyGroupOnGrid.Num())
	{
		return;
	}

	if (IsAllEnemyDead)
	{
		for (auto Enemy : EnemyGroupOnGrid)
		{
			if (Enemy)
			{
				FTimerHandle ReadyToRegenHandler;
				Enemy->IsDead(InDamageCauser);
				GetWorld()->GetTimerManager().SetTimer(ReadyToRegenHandler, FTimerDelegate::CreateLambda([&](class ACharacter* Enemy) {
					Enemy->SetActorLocation(ReadyLocation);
					Enemy->GetCharacterMovement()->GravityScale = 0.f;
					}, Enemy), MoveToReadyToRegenLocationRate, false);
			}
		}
	}
	else
	{
		if (EnemyGroupOnGrid.IsValidIndex(InIndex))
		{
			if (EnemyGroupOnGrid[InIndex])
			{
				AEnemyBase* Enemy = EnemyGroupOnGrid[InIndex];
				if (Enemy)
				{
					FTimerHandle ReadyToRegenHandler;
					GetWorld()->GetTimerManager().SetTimer(ReadyToRegenHandler, FTimerDelegate::CreateLambda([&](class ACharacter* Enemy) {
						Enemy->SetActorLocation(ReadyLocation);
						Enemy->GetCharacterMovement()->GravityScale = 0.f;
						}, Enemy), MoveToReadyToRegenLocationRate, false);
				}
			}
		}

	}

}

void AGrid::StartRegenCoolTime(uint8 InIndex)
{

	if (EnemyReadyToRespawn.IsValidIndex(InIndex) && RespawnCoolTimer.IsValidIndex(InIndex))
	{
		EnemyReadyToRespawn[InIndex] = true;
		RespawnCoolTimer[InIndex] = GetWorld()->GetTimeSeconds();
	}

}

void AGrid::CountRegenCoolTime()
{

	for (uint8 i = 0; i < EnemyReadyToRespawn.Num(); i++)
	{
		if (EnemyReadyToRespawn.IsValidIndex(i))
		{
			if (EnemyReadyToRespawn[i] == true)
			{
				if (RespawnCoolTimer.IsValidIndex(i))
				{
					float CurrentCoolTime = fabs(RespawnCoolTimer[i] - GetWorld()->GetTimeSeconds());
					if (CurrentCoolTime >= CoolTime)
					{
						RegenEnemy(i);
					}
				}
			}
		}
	}

}

void AGrid::RegenEnemy(uint8 InIndex)
{
	
	if (EnemyGroupOnGrid.IsValidIndex(InIndex))
	{
		FVector RespawnLocation;
		RespawnLocation = UKismetMathLibrary::RandomPointInBoundingBox(Box->Bounds.GetBox().GetCenter(), Box->Bounds.GetBox().GetExtent()) + FVector(0.f, 0.f, 100.f);

		if (EnemyGroupOnGrid.IsValidIndex(InIndex)
			&& EnemyReadyToRespawn.IsValidIndex(InIndex)
			&& RespawnCoolTimer.IsValidIndex(InIndex))
		{
			EnemyGroupOnGrid[InIndex]->Respawn(RespawnLocation);
			EnemyReadyToRespawn[InIndex] = false;
			RespawnCoolTimer[InIndex] = 0.f;
		}
	}
	else
	{
		PrintLine();
		return;
	}

}

// [23-08-31] Deactivate Grid While Level Seq Playing
void AGrid::HandleLevelSeqStarted()
{

	for (const auto Enemy : EnemyGroupOnGrid)
	{
		if (Enemy)
		{
			// #1. Actor
			Enemy->SetActorHiddenInGame(true);
			if (Enemy->GetWeaponComponent()->GetCurrentWeapon())
				Enemy->GetWeaponComponent()->GetCurrentWeapon()->OffSelected();

			// #3. AI
			AAIController* AI = Cast<AAIController>(Enemy->GetController());
			if (AI && AI->GetBrainComponent())
			{
				AI->GetBrainComponent()->PauseLogic("AI Pause Logic");
			}
			PrintLine();
		}
	}

	if (BossEnemy)
	{
		BossEnemy->SetActorHiddenInGame(true);
		if (BossEnemy->GetWeaponComponent()->GetCurrentWeapon())
			BossEnemy->GetWeaponComponent()->GetCurrentWeapon()->OffSelected();

		AAIController* AI = Cast<AAIController>(BossEnemy->GetController());
		if (AI && AI->GetBrainComponent())
		{
			AI->GetBrainComponent()->PauseLogic("AI Pause Logic");
		}
	}

	PrintLine();

}

// [23-08-31] Deactivate Grid While Level Seq Playing
void AGrid::HandleLevelSeqFinished()
{

	for (const auto Enemy : EnemyGroupOnGrid)
	{
		if (Enemy)
		{
			Enemy->SetActorHiddenInGame(false);

			TArray<AWeapon*> Weapons = Enemy->GetWeaponComponent()->GetWeapons();
			if (Weapons.Num() > 0 && Weapons.IsValidIndex(0))
			{
				EWeaponType WeaponType = Weapons[0]->GetWeaponType();
				Enemy->GetWeaponComponent()->EquipWeapon(WeaponType);
			}

			AAIController* AI = Cast<AAIController>(Enemy->GetController());
			if (AI && AI->GetBrainComponent())
			{
				AI->GetBrainComponent()->ResumeLogic("AI Resume Logic");
			}
		}
	}

	if (BossEnemy)
	{
		BossEnemy->SetActorHiddenInGame(false);

		TArray<AWeapon*> Weapons = BossEnemy->GetWeaponComponent()->GetWeapons();
		if (Weapons.Num() > 0 && Weapons.IsValidIndex(0))
		{
			EWeaponType WeaponType = Weapons[0]->GetWeaponType();
			BossEnemy->GetWeaponComponent()->EquipWeapon(WeaponType);
		}

		AAIController* AI = Cast<AAIController>(BossEnemy->GetController());
		if (AI && AI->GetBrainComponent())
		{
			AI->GetBrainComponent()->ResumeLogic("AI Resume Logic");
		}
	}

	DeactivateGridWhileLevelSeqPlay = false;

}
