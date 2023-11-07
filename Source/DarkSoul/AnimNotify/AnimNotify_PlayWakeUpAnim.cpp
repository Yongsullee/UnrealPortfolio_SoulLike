#include "AnimNotify_PlayWakeUpAnim.h"

#include "GameObjects/GameObject.h"

void UAnimNotify_PlayWakeUpAnim::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

	AActor* OwnerActor = MeshComp->GetOwner();
	AGameObject* OwnerObject = Cast<AGameObject>(OwnerActor);

	CheckTrue(!OwnerActor || !OwnerObject);

	if (OwnerObject->GetOwnerStateType() == EStateType::E_KnockDown)
		OwnerObject->StartWakeUp();
	else
		OwnerObject->IsExpired();
		
	
}
