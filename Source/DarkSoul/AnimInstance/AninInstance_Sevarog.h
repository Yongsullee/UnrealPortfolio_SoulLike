#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Global/Custom_Enums.h"

#include "AninInstance_Sevarog.generated.h"

UCLASS()
class DARKSOUL_API UAninInstance_Sevarog : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
		float Speed;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
		float Direction;
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
		EAIState AIState;

public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	void UpdateSpeed();
	void UpdateDirection();

private:
	void UpdateAIState();


private:
	class ACharacter* OwnerCharacter;
	
};
