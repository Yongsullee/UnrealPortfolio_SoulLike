#pragma once

#include "CoreMinimal.h"
#include "ConsumableItem.h"

#include "CI_Potion.generated.h"


UCLASS()
class DARKSOUL_API ACI_Potion : public AConsumableItem
{
	GENERATED_BODY()
	
public:	
	ACI_Potion();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class USceneComponent* Root;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class UNiagaraComponent* Potion;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class USphereComponent* Sphere;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class UNiagaraSystem* Effect;
};
