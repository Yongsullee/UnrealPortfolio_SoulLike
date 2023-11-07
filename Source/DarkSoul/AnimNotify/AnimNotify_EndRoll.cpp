// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_EndRoll.h"
#include "Global/Global.h"
#include "GameObjects/PlayerCharacter.h"

void UAnimNotify_EndRoll::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* OwnerActor = MeshComp->GetOwner();
	CheckNull(OwnerActor);

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OwnerActor);
	CheckNull(PlayerCharacter);

	PlayerCharacter->EndRoll();
}
