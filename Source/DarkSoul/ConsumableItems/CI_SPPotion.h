#pragma once

#include "CoreMinimal.h"
#include "ConsumableItems/CI_Potion.h"

#include "CI_SPPotion.generated.h"

UCLASS()
class DARKSOUL_API ACI_SPPotion : public ACI_Potion
{
	GENERATED_BODY()
	
public:
	ACI_SPPotion();

protected:
	virtual void BeginPlay() override;

private:
	virtual void StartActivation(class APlayerCharacter** OutPlayer) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Potion")
		float SPRecoveryAmount = 20.f;

};
