#pragma once

#include "CoreMinimal.h"
#include "Weapon/Weapon.h"
#include "Global/Global.h"

#include "ComboWeapon.generated.h"

UCLASS()
class DARKSOUL_API AComboWeapon : public AWeapon
{
	GENERATED_BODY()

public:
	AComboWeapon();

protected:
	virtual void BeginPlay() override;

// ************************************************************************************
		// Method
// ************************************************************************************

public:
	UFUNCTION()
		void SetEnableNextCombo(bool b);

	UFUNCTION()
		void PlayNextCombo();

	virtual void Attack()override;
	virtual void EndAttack()override;


};
