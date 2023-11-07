#include "AnimInstance_Bow.h"
#include "Global/Global.h"

#include "GameFramework/Character.h"
#include "Weapon/NoComboWeapon/Bow.h"

void UAnimInstance_Bow::NativeBeginPlay()
{
	
	Super::NativeBeginPlay();

}

void UAnimInstance_Bow::NativeUpdateAnimation(float DeltaSeconds)
{

	Super::NativeUpdateAnimation(DeltaSeconds);

	// #0. String Alpha Update
	UpdateStringAlpha();

}

void UAnimInstance_Bow::UpdateStringAlpha()
{

	AActor* OwningActor = GetOwningActor();
	CheckNull(OwningActor);

	Bow = Cast<ABow>(OwningActor);
	CheckNull(Bow);

	StringAlpha = FMath::Clamp(Bow->GetStringAlpha(), 0.f, 1.f);

}
