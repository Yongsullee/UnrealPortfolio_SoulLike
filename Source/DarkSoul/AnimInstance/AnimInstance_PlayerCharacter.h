#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Global/Global.h"
#include "ActorComponents/C_IKComponent.h"

#include "AnimInstance_PlayerCharacter.generated.h"

UCLASS()
class DARKSOUL_API UAnimInstance_PlayerCharacter : public UAnimInstance
{
	GENERATED_BODY()


public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	void UpdateSpeed();
	void UpdateDirection();
	void UpdateIK();
	void UpdateAirbone();
	void UpdateWeaponType();

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
		float Speed;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
		float Direction;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
		bool bAirBone;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
		EWeaponType WeaponType;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FFeetData FeetData;

private:
	class ACharacter* OwnerCharacter;

};
