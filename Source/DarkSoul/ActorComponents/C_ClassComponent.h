#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Global/Custom_Enums.h"

#include "C_ClassComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKSOUL_API UC_ClassComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UC_ClassComponent();

protected:
	virtual void BeginPlay() override;
		
public:
	UFUNCTION()
		void ChangeCombatType(EPlayerCombatType InType);

private:
	EPlayerCombatType CombatType = EPlayerCombatType::E_Unarmed;

public:
	FORCEINLINE EPlayerCombatType GetCombatType() { return CombatType; }
};
