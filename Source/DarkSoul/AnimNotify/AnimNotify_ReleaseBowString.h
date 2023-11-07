#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"

#include "AnimNotify_ReleaseBowString.generated.h"

UCLASS()
class DARKSOUL_API UAnimNotify_ReleaseBowString : public UAnimNotify
{

	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};
