#include "AnimNotify_EndJump.h"
#include "Global/Global.h"

#include "GameObjects/GameObject.h"

void UAnimNotify_EndJump::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* OwnerActor = MeshComp->GetOwner();
	CheckNull(OwnerActor);

	AGameObject* GameObject = Cast<AGameObject>(OwnerActor);
	CheckNull(GameObject);

	GameObject->EndJump();
}
