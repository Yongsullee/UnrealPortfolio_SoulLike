#include "AnimInstance_CastleKnight.h"
#include "Global/Global.h"

#include "GameObjects/GameObject.h"
#include "ActorComponents/C_StateComponent.h"

void UAnimInstance_CastleKnight::NativeBeginPlay()
{

	Super::NativeBeginPlay();

	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());

}

void UAnimInstance_CastleKnight::NativeUpdateAnimation(float DeltaSeconds)
{

	Super::NativeUpdateAnimation(DeltaSeconds);

	// #0 Speed & Direciton
	UpdateSpeed();
	UpdateDirection();

	// #1 AICombatState
	UpdateAIState();

}

void UAnimInstance_CastleKnight::UpdateSpeed()
{

	CheckNull(OwnerCharacter);
	// #0 Speed
	Speed = OwnerCharacter->GetVelocity().Size2D();

}

void UAnimInstance_CastleKnight::UpdateDirection()
{

	CheckNull(OwnerCharacter);

	FVector Velocity = OwnerCharacter->GetVelocity();
	FRotator Rotation = OwnerCharacter->GetControlRotation();
	Direction = CalculateDirection(Velocity, Rotation);

}

void UAnimInstance_CastleKnight::UpdateAIState()
{

	CheckNull(OwnerCharacter);

	AGameObject* OwnerObject = Cast<AGameObject>(OwnerCharacter);
	CheckTrue(!OwnerObject || !OwnerObject->GetStateComponent());

	AIState = OwnerObject->GetStateComponent()->GetAIStateType();

}
