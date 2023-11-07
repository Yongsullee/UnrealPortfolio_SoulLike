#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"

#include "AnimInstance_Bow.generated.h"

UCLASS()
class DARKSOUL_API UAnimInstance_Bow : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	void UpdateStringAlpha();

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float StringAlpha = 0.f;

	const float Max_StringAlpha = 1.f;

private:
	class ABow* Bow;

};
