// Fill out your copyright notice in the Description page of Project Settings.


#include "EnableNextCombo.h"
#include "Interface/OwnerWeaponInterface.h"
#include "Weapon/ComboWeapon/ComboWeapon.h"

void UEnableNextCombo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	AActor* PlayerActor = MeshComp->GetOwner();
	CheckNull(PlayerActor);

	if (PlayerActor->GetClass()->ImplementsInterface(UOwnerWeaponInterface::StaticClass()))
	{
		IOwnerWeaponInterface* Interface = Cast<IOwnerWeaponInterface>(PlayerActor);
		CheckNull(Interface);

		AComboWeapon* CurrentWeapon = Cast<AComboWeapon>(Interface->GetOwnerCurrentWeapon());
		CheckNull(CurrentWeapon);

		CurrentWeapon->SetEnableNextCombo(true);
	}

}

void UEnableNextCombo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* PlayerActor = MeshComp->GetOwner();
	CheckNull(PlayerActor);

	if (PlayerActor->GetClass()->ImplementsInterface(UOwnerWeaponInterface::StaticClass()))
	{
		IOwnerWeaponInterface* Interface = Cast<IOwnerWeaponInterface>(PlayerActor);
		CheckNull(Interface);

		AComboWeapon* CurrentWeapon = Cast<AComboWeapon>(Interface->GetOwnerCurrentWeapon());
		CheckNull(CurrentWeapon);

		CurrentWeapon->SetEnableNextCombo(false);
	}

}