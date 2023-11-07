#include "AnimNotify_KnockDown.h"

#include "GameObjects/GameObject.h"

void UAnimNotify_KnockDown::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

	AActor* OwnerActor = MeshComp->GetOwner();
	AGameObject* OwnerObject = Cast<AGameObject>(OwnerActor);

	CheckTrue(!OwnerActor || !OwnerObject);

	OwnerObject->KnockDown();

}
