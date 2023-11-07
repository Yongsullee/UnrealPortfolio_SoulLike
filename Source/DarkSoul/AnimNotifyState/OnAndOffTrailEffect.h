#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"

#include "OnAndOffTrailEffect.generated.h"

UCLASS()
class DARKSOUL_API UOnAndOffTrailEffect : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;

private:
	const FName EffectSocket = "TrailEffect_R";

};
