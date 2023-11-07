#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"

#include "AnimInstance_Demon.generated.h"

UCLASS()
class DARKSOUL_API UAnimInstance_Demon : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
		float Speed;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
		float Direction;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
		bool bAirBone;
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
		EAIState AIState;

public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	void UpdateSpeed();
	void UpdateDirection();
	void UpdateAirbone();

private:
	void UpdateAIState();


private:
	class ACharacter* OwnerCharacter;

};
