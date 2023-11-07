#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"

#include "AnimInstance_CastleArchery.generated.h"

UCLASS()
class DARKSOUL_API UAnimInstance_CastleArchery : public UAnimInstance
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
	void UpdateStringAlpha();

private:
	class ACharacter* OwnerCharacter;

};
