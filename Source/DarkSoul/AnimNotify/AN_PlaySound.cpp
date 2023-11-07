#include "AN_PlaySound.h"

#include "Interface/OwnerWeaponInterface.h"
#include "Weapon/Weapon.h"

void UAN_PlaySound::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

	AActor* OwnerActor = MeshComp->GetOwner();
	CheckNull(OwnerActor);

	if (OwnerActor->GetClass()->ImplementsInterface(UOwnerWeaponInterface::StaticClass()))
	{
		IOwnerWeaponInterface* Interface = Cast<IOwnerWeaponInterface>(OwnerActor);
		AWeapon* CurrentWeapon = Interface->GetOwnerCurrentWeapon();

		CheckTrue(!Interface || !CurrentWeapon);

		CurrentWeapon->PlaySound();
	}

}
