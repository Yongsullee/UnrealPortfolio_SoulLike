#include "AninInstance_Sevarog.h"
#include "Global/Global.h"

#include "GameObjects/GameObject.h"
#include "ActorComponents/C_StateComponent.h"

void UAninInstance_Sevarog::NativeBeginPlay()
{

	Super::NativeBeginPlay();

	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());

}

void UAninInstance_Sevarog::NativeUpdateAnimation(float DeltaSeconds)
{

	Super::NativeUpdateAnimation(DeltaSeconds);

	// #0 Speed & Direciton
	UpdateSpeed();
	UpdateDirection();

	// #1 AICombatState
	UpdateAIState();

}

void UAninInstance_Sevarog::UpdateSpeed()
{

	CheckNull(OwnerCharacter);

	Speed = OwnerCharacter->GetVelocity().Size2D();

}

void UAninInstance_Sevarog::UpdateDirection()
{

	CheckNull(OwnerCharacter);

	FVector Velocity = OwnerCharacter->GetVelocity();
	FRotator Rotation = OwnerCharacter->GetControlRotation();
	Direction = CalculateDirection(Velocity, Rotation);

}

void UAninInstance_Sevarog::UpdateAIState()
{

	CheckNull(OwnerCharacter);

	AGameObject* OwnerObject = Cast<AGameObject>(OwnerCharacter);
	CheckTrue(!OwnerObject || !OwnerObject->GetStateComponent());

	AIState = OwnerObject->GetStateComponent()->GetAIStateType();

}
