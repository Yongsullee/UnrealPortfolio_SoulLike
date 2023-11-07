#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"

#include "ANS_OnAndOffTrailEffect_Wing.generated.h"

UCLASS()
class DARKSOUL_API UANS_OnAndOffTrailEffect_Wing : public UAnimNotifyState
{
	GENERATED_BODY()
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;

private:
	const FName EffectSocket = "TrailEffect_Wing";

};
