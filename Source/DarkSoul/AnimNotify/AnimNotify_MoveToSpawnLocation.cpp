#include "AnimNotify_MoveToSpawnLocation.h"
#include "Global/Global.h"

#include "Interface/OwnerWeaponInterface.h"
#include "Weapon/NoComboWeapon/NoComboWeapon.h"
#include "ActorComponents/C_ProjectileSpawnerComponent.h"

void UAnimNotify_MoveToSpawnLocation::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

	AActor* OwnerActor = MeshComp->GetOwner();
	CheckNull(OwnerActor);

	if (OwnerActor->GetClass()->ImplementsInterface(UOwnerWeaponInterface::StaticClass()))
	{
		IOwnerWeaponInterface* Interface = Cast<IOwnerWeaponInterface>(OwnerActor);
		CheckTrue(!Interface);

		ANoComboWeapon* NoComboWeapon = Cast<ANoComboWeapon>(Interface->GetOwnerCurrentWeapon());
		CheckTrue(!NoComboWeapon)

		EProjectileType ProjectileType = NoComboWeapon->GetProjectileType();
		CheckTrue(ProjectileType == EProjectileType::E_Max);

		NoComboWeapon->GetProjectileSpawnerComponent()->TeleportProjectileToSpawnLocation(ProjectileType, &MeshComp, SocketName);
	}

}
