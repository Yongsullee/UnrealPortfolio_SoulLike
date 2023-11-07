#pragma once

#include "CoreMinimal.h"
#include "ConsumableItems/CI_Potion.h"

#include "CI_MPPotion.generated.h"

UCLASS()
class DARKSOUL_API ACI_MPPotion : public ACI_Potion
{
	GENERATED_BODY()
	
public:
	ACI_MPPotion();

protected:
	virtual void BeginPlay() override;

private:
	virtual void StartActivation(class APlayerCharacter** OutPlayer) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Potion")
		float MPRecoveryAmount = 20.f;

};
