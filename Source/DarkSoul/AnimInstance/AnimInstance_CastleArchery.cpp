#include "AnimInstance_CastleArchery.h"
#include "Global/Global.h"

#include "GameObjects/GameObject.h"
#include "Interface/OwnerWeaponInterface.h"
#include "ActorComponents/C_StateComponent.h"

#include "Weapon/NoComboWeapon/Bow.h"

void UAnimInstance_CastleArchery::NativeBeginPlay()
{

	Super::NativeBeginPlay();

	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());

}

void UAnimInstance_CastleArchery::NativeUpdateAnimation(float DeltaSeconds)
{

	Super::NativeUpdateAnimation(DeltaSeconds);

	// #0 Speed & Direciton
	UpdateSpeed();
	UpdateDirection();
	
	// #1 AICombatState
	UpdateAIState();

	// #2 String Alpha
	UpdateStringAlpha();

}

void UAnimInstance_CastleArchery::UpdateSpeed()
{

	CheckNull(OwnerCharacter);
	// #0 Speed
	Speed = OwnerCharacter->GetVelocity().Size2D();

}

void UAnimInstance_CastleArchery::UpdateDirection()
{

	CheckNull(OwnerCharacter);

	FVector Velocity = OwnerCharacter->GetVelocity();
	FRotator Rotation = OwnerCharacter->GetControlRotation();
	Direction = CalculateDirection(Velocity, Rotation);

}

void UAnimInstance_CastleArchery::UpdateAIState()
{
	
	CheckNull(OwnerCharacter);
	
	AGameObject* OwnerObject = Cast<AGameObject>(OwnerCharacter);
	CheckTrue(!OwnerObject);

	AIState = OwnerObject->GetStateComponent()->GetAIStateType();

}

void UAnimInstance_CastleArchery::UpdateStringAlpha()
{

	IOwnerWeaponInterface* Interface = Cast< IOwnerWeaponInterface>(OwnerCharacter);
	CheckNull(Interface);

	ABow* Bow = Cast<ABow>(Interface->GetOwnerCurrentWeapon());
	CheckNull(Bow);

	// #3 Pull Bow String
	if (AIState != EAIState::E_Idle&& Bow->IsStringReleased() == false)
		Bow->PullString();

}
