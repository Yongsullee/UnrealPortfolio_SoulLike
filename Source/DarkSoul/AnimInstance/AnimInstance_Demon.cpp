#include "AnimInstance_Demon.h"
#include "Global/Global.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameObjects/GameObject.h"
#include "ActorComponents/C_StateComponent.h"

void UAnimInstance_Demon::NativeBeginPlay()
{

	Super::NativeBeginPlay();

	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());

}

void UAnimInstance_Demon::NativeUpdateAnimation(float DeltaSeconds)
{

	Super::NativeUpdateAnimation(DeltaSeconds);

	// #0 Speed & Direciton
	UpdateSpeed();
	UpdateDirection();

	// #1. AirBone
	UpdateAirbone();

	// #2. AICombatState
	UpdateAIState();

}

void UAnimInstance_Demon::UpdateSpeed()
{

	CheckNull(OwnerCharacter);
	// #0 Speed
	Speed = OwnerCharacter->GetVelocity().Size2D();

}

void UAnimInstance_Demon::UpdateDirection()
{

	CheckNull(OwnerCharacter);

	FVector Velocity = OwnerCharacter->GetVelocity();
	FRotator Rotation = OwnerCharacter->GetControlRotation();
	Direction = CalculateDirection(Velocity, Rotation);

}

void UAnimInstance_Demon::UpdateAirbone()
{

	CheckTrue(!OwnerCharacter);

	bAirBone = OwnerCharacter->GetCharacterMovement()->IsFalling();

}

void UAnimInstance_Demon::UpdateAIState()
{

	CheckNull(OwnerCharacter);

	AGameObject* OwnerObject = Cast<AGameObject>(OwnerCharacter);
	CheckTrue(!OwnerObject || !OwnerObject->GetStateComponent());

	AIState = OwnerObject->GetStateComponent()->GetAIStateType();

}
