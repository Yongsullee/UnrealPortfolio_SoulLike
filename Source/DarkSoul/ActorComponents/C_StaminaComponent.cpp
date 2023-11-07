#include "C_StaminaComponent.h"
#include "Global/Global.h"

#include "GameObjects/GameObject.h"

UC_StaminaComponent::UC_StaminaComponent()
{

	PrimaryComponentTick.bCanEverTick = false;

	CurrentStamina = MaxStamina;
	//CurrentStamina = 10.f;

}


void UC_StaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* OwnerActor = GetOwner();
	AGameObject* OwnerObject = Cast < AGameObject>(OwnerActor);

	CheckTrue(!OwnerActor || !OwnerObject);

	OwnerObject->OnStartSprint.BindUFunction(this, "SprintHandle");

	// 0. OffSprintHandle
	GetWorld()->GetTimerManager().SetTimer(OffSprintHandle, this, &UC_StaminaComponent::RegenerateStamina, 1.f, true);
	// 1. OnSprintHandle
	GetWorld()->GetTimerManager().SetTimer(OnSprintHandle, this, &UC_StaminaComponent::DecrementStamina, 1.f, true);
	GetWorld()->GetTimerManager().PauseTimer(OnSprintHandle);

}


void UC_StaminaComponent::SprintHandle(bool IsOwnerSprinting)
{
	if (IsOwnerSprinting == true)
	{
		// #1. Decrement Stamina
		GetWorld()->GetTimerManager().PauseTimer(OffSprintHandle);
		GetWorld()->GetTimerManager().UnPauseTimer(OnSprintHandle);
	}
	else
	{
		// #2. Regenerate Stamina
		GetWorld()->GetTimerManager().PauseTimer(OnSprintHandle);
		GetWorld()->GetTimerManager().UnPauseTimer(OffSprintHandle);
	}

}

void UC_StaminaComponent::RegenerateStamina()
{
	// 1. Clamping
	CurrentStamina = FMath::Clamp(CurrentStamina+BaseStaminaIncrementValue, 0.f, MaxStamina);

	OnUpdateHUDProfileStaminaBar.ExecuteIfBound(MaxStamina, CurrentStamina);
}

void UC_StaminaComponent::DecrementStamina()
{
	CurrentStamina = FMath::Clamp(CurrentStamina - BaseStaminaDecrementValue, 0.f, MaxStamina);

	OnUpdateHUDProfileStaminaBar.ExecuteIfBound(MaxStamina, CurrentStamina);
}

void UC_StaminaComponent::RecoverCurrentStamina(float InRecoveryAmount)
{
	// 1. Clamping
	CurrentStamina = FMath::Clamp(CurrentStamina + InRecoveryAmount, 0.f, MaxStamina);

	OnUpdateHUDProfileStaminaBar.ExecuteIfBound(MaxStamina, CurrentStamina);
}

