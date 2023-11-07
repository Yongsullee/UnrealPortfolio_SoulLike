#include "AnimNotify_EndAttack.h"

#include "GameObjects/GameObject.h"

void UAnimNotify_EndAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

	AActor* OwnerActor = MeshComp->GetOwner();
	AGameObject* OwnerObject = Cast<AGameObject>(OwnerActor);

	if (!!OwnerActor && !!OwnerObject)
	{
		OwnerObject->EndAttack();
	}

}