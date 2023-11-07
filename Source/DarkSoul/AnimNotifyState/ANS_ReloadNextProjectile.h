#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"

#include "ANS_ReloadNextProjectile.generated.h"

UCLASS()
class DARKSOUL_API UANS_ReloadNextProjectile : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

private:
	const FName BowSocket = TEXT("Bow_Projectile");
	const FName HandSocket = TEXT("NextProjectile");
};
