#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "C_StaminaComponent.generated.h"

DECLARE_DELEGATE_TwoParams(FUpdateHUDProfileStaminaBar, float, float);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKSOUL_API UC_StaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UC_StaminaComponent();

protected:
	virtual void BeginPlay() override;


//	******************************************************************************************
		// Method
//	******************************************************************************************

public:
	UFUNCTION()
		void SprintHandle(bool IsOwnerSprinting);
private:
	void RegenerateStamina();
	void DecrementStamina();

public:
	void RecoverCurrentStamina(float InRecoveryAmount);

//	******************************************************************************************
		// Member
//	******************************************************************************************

private:

	UPROPERTY(EditDefaultsOnly, Category = "Stamina")
		float MaxStamina = 100.f;
	UPROPERTY(EditDefaultsOnly, Category = "Stamina")
		float BaseStaminaIncrementValue =1.f;
	UPROPERTY(EditDefaultsOnly, Category = "Stamina")
		float BaseStaminaDecrementValue = 5.f;
	UPROPERTY(VisibleAnywhere, Category = "Stamina")
		float CurrentStamina = 0.f;

	FTimerHandle OnSprintHandle;
	FTimerHandle OffSprintHandle;

public:
	FUpdateHUDProfileStaminaBar OnUpdateHUDProfileStaminaBar;

//	******************************************************************************************
		// Getter & Setter
//	******************************************************************************************

public:
	FORCEINLINE float GetCurrentStamina() { return CurrentStamina; }

		
};
