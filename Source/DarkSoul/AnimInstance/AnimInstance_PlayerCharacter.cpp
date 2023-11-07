#include "AnimInstance_PlayerCharacter.h"

#include "GameFramework/Character.h"
#include "GameObjects/GameObject.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "ActorComponents/C_WeaponComponent.h"

#include "ActorComponents/C_IKComponent.h"

void UAnimInstance_PlayerCharacter::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());

}

void UAnimInstance_PlayerCharacter::NativeUpdateAnimation(float DeltaSeconds)
{

	Super::NativeUpdateAnimation(DeltaSeconds);

	UpdateSpeed();

	UpdateDirection();

	UpdateAirbone();

	UpdateWeaponType();

	UpdateIK();

}

void UAnimInstance_PlayerCharacter::UpdateSpeed()
{

	CheckTrue(!OwnerCharacter);

	Speed = OwnerCharacter->GetVelocity().Size2D();

}

void UAnimInstance_PlayerCharacter::UpdateDirection()
{

	CheckTrue(!OwnerCharacter);

	FVector Velocity = OwnerCharacter->GetVelocity();
	FRotator Rotation = OwnerCharacter->GetControlRotation();
	Direction = CalculateDirection(Velocity, Rotation);

}

void UAnimInstance_PlayerCharacter::UpdateIK()
{

	CheckTrue(!OwnerCharacter);

	UC_IKComponent* feet = CHelpers::GetComponent<UC_IKComponent>(OwnerCharacter);
	if (!!feet)
		FeetData = feet->GetData();

}

void UAnimInstance_PlayerCharacter::UpdateAirbone()
{

	CheckTrue(!OwnerCharacter);

	bAirBone = OwnerCharacter->GetCharacterMovement()->IsFalling();

}

void UAnimInstance_PlayerCharacter::UpdateWeaponType()
{

	AGameObject* OwnerObject = Cast<AGameObject>(OwnerCharacter);

	CheckTrue(!OwnerCharacter || !OwnerObject);

	WeaponType = OwnerObject->GetWeaponComponent()->GetWeaponType();

}
