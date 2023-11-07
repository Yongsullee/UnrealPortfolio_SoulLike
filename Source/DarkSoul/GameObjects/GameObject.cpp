#include "GameObjects/GameObject.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/ArrowComponent.h"
#include "ActorComponents/C_HealthComponent.h"
#include "ActorComponents/C_ManaComponent.h"
#include "ActorComponents/C_StateComponent.h"
#include "ActorComponents/C_WeaponComponent.h"
#include "ActorComponents/C_IKComponent.h"
#include "ActorComponents/C_MontageComponent.h"
#include "ActorComponents/C_RewardComponent.h"
#include "ActorComponents/C_VaultComponent.h"
#include "ActorComponents/C_StaminaComponent.h"
#include "ActorComponents/C_PowerComponent.h"
#include "ActorComponents/C_FXComponent.h"
#include "ActorComponents/C_AudioComponent.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "AIController/AIManager.h"
#include "Sound/SoundBase.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"

AGameObject::AGameObject()
{
	
	PrimaryActorTick.bCanEverTick = false;

	StateComponent = CreateDefaultSubobject<UC_StateComponent>(TEXT("State Component"));
	HealthComponent = CreateDefaultSubobject<UC_HealthComponent>(TEXT("Health Component"));
	ManaComponent = CreateDefaultSubobject<UC_ManaComponent>(TEXT("Mana Component"));
	StaminaComponent = CreateDefaultSubobject<UC_StaminaComponent>(TEXT("Stamina Component"));
	WeaponComponent = CreateDefaultSubobject<UC_WeaponComponent>(TEXT("Weapon Component"));
	VaultComponent = CreateDefaultSubobject<UC_VaultComponent>(TEXT("Vault Component"));
	IKComponent = CreateDefaultSubobject<UC_IKComponent>(TEXT("IK Component"));
	MontageComponent = CreateDefaultSubobject<UC_MontageComponent>(TEXT("Montage Component"));
	RewardComponent = CreateDefaultSubobject<UC_RewardComponent>(TEXT("Reward Component"));
	FXComponent = CreateDefaultSubobject<UC_FXComponent>(TEXT("FX Component"));
	AudioComponent = CreateDefaultSubobject<UC_AudioComponent>(TEXT("Audio Component"));

	VaultArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Vault Arrow"));
	VaultArrow->SetupAttachment(GetMesh());
	VaultArrow->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	VaultArrow->SetRelativeLocation(FVector(0.f, 0.f, 50.f));

	// Avoidance
	GetCharacterMovement()->bUseRVOAvoidance = true;
	GetCharacterMovement()->AvoidanceConsiderationRadius = 100.f;
	GetCharacterMovement()->AvoidanceWeight = 0.1f;

}


void AGameObject::BeginPlay()
{

	Super::BeginPlay();

	// #0. Material
	UMaterialInstanceConstant* blood;
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&blood, "MaterialInstanceConstant'/Game/mi_blood_water_bright.mi_blood_water_bright'");
	BloodMaterial = UMaterialInstanceDynamic::Create(blood, this);

	// #1. AI
	AAIController* AI= Cast<AAIController>(GetController());
	if (!!AI)
	{
		AIController = AI;
	}

	// #2. Delegate
	HealthComponent->OnStartGuardSkillBlock.BindUFunction(this, "GuardSkill_Block");
	HealthComponent->OnOwnerIsDamaged.BindUFunction(this, "IsStartDamaged");
	HealthComponent->OnOwnerIsDead.BindUFunction(this, "IsDead");

}


void AGameObject::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// 0. Combat			****************************************************

void AGameObject::StartJump()
{

	CheckTrue(!StateComponent);

	if (StateComponent->GetStateType() == EStateType::E_Idle)
	{
		JumpCount++;

		ACharacter::Jump();

		OnChangeStateType.ExecuteIfBound(EStateType::E_Jumping);

	}
	else if(StateComponent->GetStateType() == EStateType::E_Jumping)
	{
		JumpCount++;

		if (JumpCount >= MaxJumpCount)
		{
			DoubleJump();
			JumpCount = 0;
		}
	}

}


void AGameObject::EndJump()
{

	OnChangeStateType.ExecuteIfBound(EStateType::E_Idle);

	JumpCount = 0;

}


void AGameObject::OnSprint()
{

	CheckTrue(!StateComponent);

	// Idle + IsNotAirBone + Stamina > 10.f
	if (StateComponent->GetStateType() == EStateType::E_Idle
		&& GetCharacterMovement()->IsFalling() == false
		&& StaminaComponent->GetCurrentStamina() > 10.f
		)
	{
		// 0. Speed
		GetCharacterMovement()->MaxWalkSpeed = 600.f;
		// 2. Stamina
		OnStartSprint.ExecuteIfBound(true);
	}

}


void AGameObject::OffSprint()
{

	if (StateComponent->GetStateType() == EStateType::E_Idle
		&& GetCharacterMovement()->IsFalling() == false)
	{
		// 0. Speed
		GetCharacterMovement()->MaxWalkSpeed = 400.f;
		//// 2. Stamina
		OnStartSprint.ExecuteIfBound(false);
	}

}


void AGameObject::StartVault()
{

	CheckTrue(!StateComponent);

	if (StateComponent->GetStateType() == EStateType::E_Idle
		|| StateComponent->GetStateType() == EStateType::E_Jumping)
	{
		OnStartVault.ExecuteIfBound();
	}

}


void AGameObject::DoubleJump()
{

	OnDoubleJump.ExecuteIfBound();

}

void AGameObject::Attack()
{

	// #0. Multicast
	OnWeaponAttack.Broadcast();

	// #1. AI
	if(AIController)
		AIController->GetBrainComponent()->PauseLogic("Agent Is Attacking");


}

void AGameObject::EndAttack()
{

	// #0. Multicast
	OnWeaponEndAttack.Broadcast();

	// #1. AI
	if (AIController)
		AIController->GetBrainComponent()->ResumeLogic("Agent Is Idle");

}

void AGameObject::AttackSkill_00()
{

	// #0. Multicast
	OnWeaponAttackSkill.Broadcast(0);

	// #1. AI
	if (!!AIController)
		AIController->GetBrainComponent()->PauseLogic("Agent Is Attacking");

}

void AGameObject::AttackSkill_01()
{

	// #0. Multicast
	OnWeaponAttackSkill.Broadcast(1);

	// #1. AI
	if (!!AIController)
		AIController->GetBrainComponent()->PauseLogic("Agent Is Attacking");

}

void AGameObject::AttackSkill_02()
{

	// #0. Multicast
	OnWeaponAttackSkill.Broadcast(2);

	// #1. AI
	if (!!AIController)
		AIController->GetBrainComponent()->PauseLogic("Agent Is Attacking");

}

void AGameObject::EndAttackSkill()
{

	// #0. Multicast
	OnWeaponEndAttackSkill.Broadcast();

	// #1. AI
	if (!!AIController)
		AIController->GetBrainComponent()->ResumeLogic("Agent Is Idle");

}

void AGameObject::GuardSkill()
{

	//#0. Multicast
	OnWeaponGuardSkill.Broadcast();

	// #1. AI
	if (!!AIController)
		AIController->GetBrainComponent()->PauseLogic("Agent Is Guarding");

}

void AGameObject::GuardSkill_Block()
{

	// #0. Multicast
	OnWeaponGuardSkill_Block.Broadcast();

}

void AGameObject::EndGuardSkill()
{

	// #0. Multicast
	OnWeaponEndGuardSkill.Broadcast();

	// #1. AI
	if (!!AIController)
		AIController->GetBrainComponent()->ResumeLogic("Agent Is Idle");

}


bool AGameObject::IsHostile(AGameObject* InObject)
{
	return (Faction != InObject->GetFaction() && InObject->Faction != EFaction::E_Neutral);
}

/*	************************************************************************************************
*	
*	[Remark]	:	Actor --- Wall --- Actor 상황에서, AIPerception이 작동하지 않는 문제를 해결합니다.
*	링크			:	https://www.youtube.com/watch?v=anEUhgOTKBY&ab_channel=PranjalBhattacharjee
* 	
*	************************************************************************************************/

// [23-07-20] : Sight Figure
bool AGameObject::CanBeSeenFrom(const FVector& ObserverLocation, FVector& OutSeenLocation, int32& NumberOfLoSChecksPerformed, float& OutSightStrength, const AActor* IgnoreActor, const bool* bWasVisible, int32* UserData) const
{

	static const FName NAME_AILineOfSight = FName(TEXT("TestPawnLineOfSight"));
	FHitResult HitResult;
	FVector SocketLocation = GetMesh()->GetSocketLocation(PerceptionTarget);

	const bool bHitSocket = GetWorld()->LineTraceSingleByObjectType(HitResult, ObserverLocation, SocketLocation,
		FCollisionObjectQueryParams(ECC_TO_BITFIELD
		(ECC_WorldStatic) | ECC_TO_BITFIELD(ECC_WorldDynamic)),
		FCollisionQueryParams(NAME_AILineOfSight, true,
			IgnoreActor));

	NumberOfLoSChecksPerformed++;

	if (bHitSocket == false || (HitResult.GetActor() && HitResult.GetActor()->IsOwnedBy(this)))
	{
		OutSeenLocation = SocketLocation;
		OutSightStrength = 1;

		return true;
	}

	const bool bHit = GetWorld()->LineTraceSingleByObjectType(HitResult, ObserverLocation, GetActorLocation(),
		FCollisionObjectQueryParams(ECC_TO_BITFIELD(ECC_WorldStatic) |
			ECC_TO_BITFIELD(ECC_WorldDynamic)), FCollisionQueryParams
			(NAME_AILineOfSight, true, IgnoreActor));

	NumberOfLoSChecksPerformed++;

	if (bHit == false || (HitResult.GetActor() && HitResult.GetActor()->IsOwnedBy(this)))
	{
		OutSeenLocation = GetActorLocation();
		OutSightStrength = 1;

		return true;
	}

	OutSightStrength = 0;
	return false;

}


/*	***************************************************************************************************
* 
*	[Remark]	: InDamgaeCauser(Weapon)을 소유하는 Owner(GameObject)에의해 전달 받은 데미지를 구현합니다.
*
*	***************************************************************************************************/

void AGameObject::IsStartDamaged(AActor* InDamageCauser, float InImpulseAmount, bool bStatusEffectTriggered)
{

	if (GetStateComponent()->GetStateType() != EStateType::E_Jumping
		&& GetStateComponent()->GetStateType() != EStateType::E_Skilling
		&& GetStateComponent()->GetStateType() != EStateType::E_KnockDown
		&& GetStateComponent()->GetStateType() != EStateType::E_WakeUp
		&& GetStateComponent()->GetStateType() != EStateType::E_Dead
		&& InDamageCauser)
	{
		AActor* DamageCauserOwner = InDamageCauser->GetOwner();
		if (DamageCauserOwner)
		{
			ACharacter* DamageCauserOwnerCharacter = Cast<ACharacter>(InDamageCauser->GetOwner());
			if (DamageCauserOwnerCharacter && DamageCauserOwnerCharacter != this)
			{
				if (InImpulseAmount >= KnockDownImpulse)
				{
					// State
					OnChangeStateType.ExecuteIfBound(EStateType::E_KnockDown);
					// Movement
					GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
					// Montage
					if (WeaponComponent)
					{
						OnPlayDeathAnim.ExecuteIfBound(
							EStateType::E_Dead,
							WeaponComponent->GetWeaponType(),
							WeaponComponent->GetCurrentWeapon()->GetComboType(),
							-1,
							-1
						);
					}

				}
				else
				{
					// State
					OnChangeStateType.ExecuteIfBound(EStateType::E_Stumbling);
					// Movement
					GetCharacterMovement()->StopActiveMovement();
					// Montage Comp
					if (StateComponent && WeaponComponent)
					{
						OnPlayHitAnim.ExecuteIfBound(
							StateComponent->GetStateType(),
							WeaponComponent->GetWeaponType(),
							WeaponComponent->GetCurrentWeapon()->GetComboType(),
							-1,
							-1
						);
					}
				}
				// Knock Back
				KnockBack(DamageCauserOwnerCharacter, InImpulseAmount);
				// Sound
				OnPlayHumanSound.ExecuteIfBound(
					this,
					StateComponent->GetStateType(),
					WeaponComponent->GetWeaponType(),
					WeaponComponent->GetCurrentWeapon()->GetComboType(),
					-1,
					-1
				);
				// AI
				if (AIController)
				{
					if(AIController->GetBrainComponent()->IsActive())
						AIController->GetBrainComponent()->PauseLogic("Agent Is Stumbling");
				}
			}
		}
	}

}

void AGameObject::LookAtDamageCauser(ACharacter** InCharacter)
{

	if (!!(*InCharacter))
	{
		FVector TargetLocation = (*InCharacter)->GetActorLocation();
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLocation);
		LookAtRotation.Pitch = 0.f;
		SetActorRotation(LookAtRotation);
	}

}

void AGameObject::KnockBack(ACharacter* InCharacter, int32 InNum)
{

	FVector TargetLocation = GetActorLocation() + (InCharacter->GetActorForwardVector() * InNum);
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;

	UKismetSystemLibrary::MoveComponentTo(
		RootComponent,
		TargetLocation,
		GetActorRotation(),
		false,
		false,
		0.15f,
		true,
		EMoveComponentAction::Move,
		LatentInfo
	);

}

void AGameObject::KnockDown()
{

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	// #1. Montage
	OnPlayKnockDownAnim.ExecuteIfBound(
		EStateType::E_KnockDown,
		WeaponComponent->GetWeaponType(),
		WeaponComponent->GetCurrentWeapon()->GetComboType(),
		-1,
		-1
	);

	// #2. Sound
	OnPlayHumanSound.ExecuteIfBound(
		this,
		EStateType::E_KnockDown,
		WeaponComponent->GetWeaponType(),
		WeaponComponent->GetCurrentWeapon()->GetComboType(),
		-1,
		-1
	);

}

void AGameObject::StartWakeUp()
{

	// #1. Wake Up
	OnPlayWakeUpAnim.ExecuteIfBound(
		EStateType::E_WakeUp,
		WeaponComponent->GetWeaponType(),
		WeaponComponent->GetCurrentWeapon()->GetComboType(),
		-1,
		-1
	);

}

void AGameObject::IsExpired()
{

	SetActorHiddenInGame(true);
	WeaponComponent->GetCurrentWeapon()->OffSelected();

	GetCharacterMovement()->GravityScale = 0.f;

}

void AGameObject::IsEndDamaged()
{
	
	EStateType CurrentStateType = StateComponent->GetStateType();

	// #0. State
	OnChangeStateType.ExecuteIfBound(EStateType::E_Idle);

	// #1. AI
	if (!!AIController && AIController->GetBrainComponent()->IsPaused())
	{
		AIController->GetBrainComponent()->ResumeLogic("Agent Is Idle");
	}

	// #2. Rotation
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// #3. Character Movement
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

}

/*	*******************************************************************************************
* 
*	[Remark]	:	Dead 상태로 전환되기 전에 Impulse를 적용합니다.
*	
*	목적:
*		Dead 상태로 전환 후 Impulse가 적용되지 않고, 제자리에서 Dead 상태로 전환되는 문제를 방지합니다.
* 
*	설명:
*		1. Impulse Amount 만큼 이동 동작 수행
*		2. 그 후 이전 코드 그대로 적용
* 
*	*******************************************************************************************/

// [23-08-10] : Status Effect 수정
void AGameObject::IsDead(AActor* InDamageCauser, float InImpulseAmount, bool bStatusEffectTriggered)
{

	if (InDamageCauser)
	{
		AActor* DamageCauserOwner = InDamageCauser->GetOwner();
		if (DamageCauserOwner)
		{
			ACharacter* DamageCauserCharacter = Cast<ACharacter>(DamageCauserOwner);
			if (DamageCauserCharacter)
			{
				if (StateComponent->GetStateType() == EStateType::E_Idle)
				{
					if (DamageCauserCharacter != this)
					{
						if (InImpulseAmount > 0.f) KnockBack(DamageCauserCharacter, InImpulseAmount);
					}
					GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
					OnChangeStateType.ExecuteIfBound(EStateType::E_Dead);
				}
				// Montage
				OnPlayDeathAnim.ExecuteIfBound(
					StateComponent->GetStateType(),
					WeaponComponent->GetWeaponType(),
					WeaponComponent->GetCurrentWeapon()->GetComboType(),
					-1,
					-1
				);
				// Sound
				OnPlayHumanSound.ExecuteIfBound(
					this,
					StateComponent->GetStateType(),
					WeaponComponent->GetWeaponType(),
					WeaponComponent->GetCurrentWeapon()->GetComboType(),
					-1,
					-1
				);
				// # AI
				if (AIController && AIController->GetBrainComponent())
				{
					AIController->GetBrainComponent()->PauseLogic("Agent Is Dead");
					AIController->ClearFocus(EAIFocusPriority::LastFocusPriority);
				}
			}

		}
	}

}

void AGameObject::Respawn(const FVector& InLocation)
{

	// #0. State Type
	OnChangeStateType.ExecuteIfBound(EStateType::E_Idle);
	// #1. Location
	TeleportTo(InLocation, FRotator::ZeroRotator);
	// #2. Character Movement
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	GetCharacterMovement()->GravityScale = 1.f;
	// #4. AI
	if (AIController)
		AIController->GetBrainComponent()->ResumeLogic("Agent Is Respawned");
	// #6. Health
	if (HealthComponent)
	{
		HealthComponent->RecoverCurrentHealth(200.f);
	}
	// #7. Visibility
	if(WeaponComponent)
		WeaponComponent->GetCurrentWeapon()->OnSelected();

	SetActorHiddenInGame(false);

}

void AGameObject::ChangeColor()
{

	if (!!BloodMaterial && GetMesh()->GetMaterial(0))
	{
		GetMesh()->SetMaterial(0, BloodMaterial);
		GetWorld()->GetTimerManager().SetTimer(ChangeMaterialHandler, FTimerDelegate::CreateLambda([&]() {
			RestoreColor();
			}), 0.1f, false);
	}
	else
	{
		return;
	}

}


/****************************************************************************************************
* 
*	Interface
* 
******************************************************************************************************/

EStateType AGameObject::GetOwnerStateType()
{
	return StateComponent->GetStateType();
}

void AGameObject::SetOwnerStateType(EStateType StateTypeVal)
{
	StateComponent->SetStateType(StateTypeVal);
}

EComboType AGameObject::GetOwnerComboType()
{
	return WeaponComponent->GetCurrentWeapon()->GetComboType();
}

AWeapon* AGameObject::GetOwnerCurrentWeapon()
{
	return WeaponComponent->GetCurrentWeapon();
}