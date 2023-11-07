#include "AnimNotify_EndDamaged.h"
#include "Global/Global.h"

#include "GameObjects/GameObject.h"

void UAnimNotify_EndDamaged::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* OwnerActor = MeshComp->GetOwner();
	CheckNull(OwnerActor);

	AGameObject* GameObject = Cast<AGameObject>(OwnerActor);
	CheckNull(GameObject);
	
	GameObject->IsEndDamaged();
}
