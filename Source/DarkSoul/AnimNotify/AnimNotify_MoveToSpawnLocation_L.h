#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_MoveToSpawnLocation_L.generated.h"

UCLASS()
class DARKSOUL_API UAnimNotify_MoveToSpawnLocation_L : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

private:
	const FName SocketName = TEXT("Projectile_LeftHand");
};
