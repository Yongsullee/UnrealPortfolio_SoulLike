#include "AnimNotify_SpawnMagicProjectile_L.h"
#include "Global/Global.h"

#include "GameObjects/GameObject.h"
#include "Weapon/NoComboWeapon/Magic.h"

void UAnimNotify_SpawnMagicProjectile_L::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

	AActor* OwnerActor = MeshComp->GetOwner();
	CheckNull(OwnerActor);

	IOwnerWeaponInterface* Interface = Cast<IOwnerWeaponInterface>(OwnerActor);
	CheckNull(Interface);

	AMagic* Magic = Cast<AMagic>(Interface->GetOwnerCurrentWeapon());
	CheckNull(Magic);

	Magic->LaunchProjectile();

}
