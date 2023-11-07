#include "GameObjects/Sevarog.h"
#include "Global/Global.h"

#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"

#include "ActorComponents/C_ManaComponent.h"
#include "ActorComponents/C_StateComponent.h"
#include "ActorComponents/C_WeaponComponent.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "AIController/AIC_EnemyBase.h"
#include "BrainComponent.h"

ASevarog::ASevarog()
{

	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->InitCapsuleSize(100.f, 150.f);

	GetMesh()->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -128.f));

	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.0f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	HUDComponent->SetRelativeScale3D(FVector(8.f, 8.f, 8.f));
	HUDComponent->SetRelativeLocation(FVector(0.f, 0.f, 300.f));

	// Animation Blueprint
	TSubclassOf<UAnimInstance> animInstance;
	CHelpers::GetClass<UAnimInstance>(&animInstance, "AnimBlueprint'/Game/Blueprints/Enemy/Sevarog/ABP_Sevarog'");
	GetMesh()->SetAnimInstanceClass(animInstance);

}

void ASevarog::BeginPlay()
{
	Super::BeginPlay();

	// Test
	//FTimerHandle TestHandler;
	//GetWorld()->GetTimerManager().SetTimer(TestHandler, FTimerDelegate::CreateLambda([&]() {
	//	AttackSkill_02();
	//	}), 3.f, true);

}


void ASevarog::RunAICombatPhase(EAICombatPhase InEnum)
{

	Super::RunAICombatPhase(InEnum);

	switch (InEnum)
	{
	case EAICombatPhase::E_Phase1:
		AICombatPhase01();
		break;
	case EAICombatPhase::E_Phase2:
		AICombatPhase02();
		break;
	case EAICombatPhase::E_Phase3:
		AICombatPhase03();
		break;
	}

}


int ASevarog::GetValidSkillInAICombatPhase(int8 InAICombatPhase)
{

	switch (InAICombatPhase)
	{
	case 0:
		if (GetManaComponent()->GetAttackSillValid(0))
		{
			return 0;
		}
		return -1;
		break;
	case 1:
		if (GetManaComponent()->GetAttackSillValid(1))
		{
			return 1;
		}
		if (GetManaComponent()->GetAttackSillValid(0))
		{
			return 0;
		}
		return -1;
		break;
	case 2:
		if (GetManaComponent()->GetAttackSillValid(2))
		{
			return 2;
		}
		if (GetManaComponent()->GetAttackSillValid(1))
		{
			return 1;
		}
		if (GetManaComponent()->GetAttackSillValid(0))
		{
			return 0;
		}
		return -1;
		break;
	case 3:
		return -1;
		break;
	}

	return -1;

}

void ASevarog::AICombatPhase01()
{

	if (GetManaComponent()->GetAttackSillValid(0))
	{
		AttackSkill_00();
	}

}

void ASevarog::AICombatPhase02()
{

	if (GetManaComponent()->GetAttackSillValid(1))
	{
		AttackSkill_01();
	}
	else if (GetManaComponent()->GetAttackSillValid(0))
	{
		AttackSkill_00();
	}

}

void ASevarog::AICombatPhase03()
{

	if (GetManaComponent()->GetAttackSillValid(2))
	{
		AttackSkill_02();
	}
	else if (GetManaComponent()->GetAttackSillValid(0))
	{
		AttackSkill_00();
	}

}
