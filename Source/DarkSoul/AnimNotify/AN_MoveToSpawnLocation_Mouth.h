#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"

#include "AN_MoveToSpawnLocation_Mouth.generated.h"

UCLASS()
class DARKSOUL_API UAN_MoveToSpawnLocation_Mouth : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

private:
	const FName SocketName = TEXT("Projectile_Mouth");

};
