#include "AnimNotify_EndGuardSkill.h"
#include "Global/Global.h"

#include "GameObjects/GameObject.h"


void UAnimNotify_EndGuardSkill::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* OwnerActor = MeshComp->GetOwner();
	CheckNull(OwnerActor);

	AGameObject* OwnerObject = Cast<AGameObject>(OwnerActor);
	CheckNull(OwnerObject);

	OwnerObject->EndGuardSkill();
}
