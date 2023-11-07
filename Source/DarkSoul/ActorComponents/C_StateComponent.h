#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Global/Custom_Enums.h"

#include "C_StateComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKSOUL_API UC_StateComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	UC_StateComponent();

protected:
	virtual void BeginPlay() override;

private:
	EStateType OwnerStateType = EStateType::E_Idle;
	EAIState OwnerAIStateType = EAIState::E_Idle;

public:
	UFUNCTION()
		void SetStateType(EStateType InType) { OwnerStateType = InType; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE EStateType GetStateType() { return OwnerStateType; }
	UFUNCTION()
		void SetAIStateType(EAIState InType) { OwnerAIStateType = InType; }
	UFUNCTION(BlueprintPure)
		FORCEINLINE EAIState GetAIStateType() { return OwnerAIStateType; }



};
