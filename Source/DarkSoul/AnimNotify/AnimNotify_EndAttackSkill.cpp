#include "AnimNotify_EndAttackSkill.h"
#include "Global/Global.h"

#include "GameObjects/GameObject.h"

void UAnimNotify_EndAttackSkill::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	
	AActor* OwnerActor = MeshComp->GetOwner();
	CheckTrue(!OwnerActor);

	AGameObject* OwnerObject = Cast<AGameObject>(OwnerActor);
	CheckTrue(!OwnerObject);

	OwnerObject->EndAttackSkill();

}
