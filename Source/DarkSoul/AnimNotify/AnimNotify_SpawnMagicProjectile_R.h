#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"

#include "AnimNotify_SpawnMagicProjectile_R.generated.h"

UCLASS()
class DARKSOUL_API UAnimNotify_SpawnMagicProjectile_R : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

};
