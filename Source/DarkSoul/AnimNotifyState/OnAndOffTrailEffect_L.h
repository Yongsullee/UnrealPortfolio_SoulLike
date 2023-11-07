#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"

#include "OnAndOffTrailEffect_L.generated.h"

UCLASS()
class DARKSOUL_API UOnAndOffTrailEffect_L : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;

private:
	const FName EffectSocket = "TrailEffect_L";

};
