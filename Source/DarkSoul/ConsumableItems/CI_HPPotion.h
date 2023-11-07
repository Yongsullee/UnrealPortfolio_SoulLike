#pragma once

#include "CoreMinimal.h"
#include "ConsumableItems/CI_Potion.h"

#include "CI_HPPotion.generated.h"

UCLASS()
class DARKSOUL_API ACI_HPPotion : public ACI_Potion
{
	GENERATED_BODY()

public:
	ACI_HPPotion();

protected:
	virtual void BeginPlay() override;

private:
	virtual void StartActivation(class APlayerCharacter** OutPlayer) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Potion")
		float HPRecoveryAmount = 20.f;

};
