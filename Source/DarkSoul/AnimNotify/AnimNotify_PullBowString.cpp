#include "AnimNotify_PullBowString.h"

#include "Interface/OwnerWeaponInterface.h"
#include "Weapon/Weapon.h"
#include "Weapon/NoComboWeapon/Bow.h"

void UAnimNotify_PullBowString::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

	AActor* OwnerActor = MeshComp->GetOwner();
	CheckNull(OwnerActor);

	if (OwnerActor->GetClass()->ImplementsInterface(UOwnerWeaponInterface::StaticClass()))
	{
		IOwnerWeaponInterface* Interface = Cast<IOwnerWeaponInterface>(OwnerActor);
		CheckNull(Interface);

		AWeapon* CurrentWeapon = Interface->GetOwnerCurrentWeapon();
		CheckNull(CurrentWeapon);

		ABow* Bow = Cast<ABow>(CurrentWeapon);
		CheckNull(Bow);

		Bow->PullString();
	}
}
