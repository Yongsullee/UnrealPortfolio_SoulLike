#include "AN_LaunchMagicProjectile.h"
#include "Global/Global.h"

#include "Interface/OwnerWeaponInterface.h"
#include "Weapon/NoComboWeapon/NoComboWeapon.h"

void UAN_LaunchMagicProjectile::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

	AActor* OwnerActor = MeshComp->GetOwner();
	CheckTrue(!OwnerActor);

	IOwnerWeaponInterface* Interface = Cast<IOwnerWeaponInterface>(OwnerActor);
	CheckTrue(!Interface);

	ANoComboWeapon* NoComboWeapon = Cast<ANoComboWeapon>(Interface->GetOwnerCurrentWeapon());
	CheckTrue(!NoComboWeapon)

	NoComboWeapon->LaunchProjectile();

}
