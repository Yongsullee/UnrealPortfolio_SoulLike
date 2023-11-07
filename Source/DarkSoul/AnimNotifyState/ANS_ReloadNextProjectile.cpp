#include "ANS_ReloadNextProjectile.h"
#include "Global/Global.h"

#include "Interface/OwnerWeaponInterface.h"
#include "Weapon/NoComboWeapon/Bow.h"
#include "ActorComponents/C_ProjectileSpawnerComponent.h"

void UANS_ReloadNextProjectile::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{

	AActor* OwnerActor = MeshComp->GetOwner();
	CheckNull(OwnerActor);

	if (OwnerActor->GetClass()->ImplementsInterface(UOwnerWeaponInterface::StaticClass()))
	{
		IOwnerWeaponInterface* Interface = Cast<IOwnerWeaponInterface>(OwnerActor);
		ANoComboWeapon* NoComboWeapon = Cast<ANoComboWeapon>(Interface->GetOwnerCurrentWeapon());
		EProjectileType ProjectileType = NoComboWeapon->GetProjectileType();

		CheckTrue(!Interface || !NoComboWeapon || (ProjectileType == EProjectileType::E_Max));

		NoComboWeapon->GetProjectileSpawnerComponent()->AttachProjectile(ProjectileType, &MeshComp, HandSocket);
	}

}

void UANS_ReloadNextProjectile::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

	AActor* OwnerActor = MeshComp->GetOwner();
	CheckNull(OwnerActor);

	if (OwnerActor->GetClass()->ImplementsInterface(UOwnerWeaponInterface::StaticClass()))
	{
		IOwnerWeaponInterface* Interface = Cast<IOwnerWeaponInterface>(OwnerActor);
		ABow* Bow = Cast<ABow>(Interface->GetOwnerCurrentWeapon());
		EProjectileType ProjectileType = Bow->GetProjectileType();
		USkeletalMeshComponent* SkeletalMesh = Bow->GetSkeletalMesh();

		CheckTrue(!Interface || !Bow || (ProjectileType == EProjectileType::E_Max || !SkeletalMesh));

		Bow->GetProjectileSpawnerComponent()->AttachProjectile(ProjectileType, &SkeletalMesh, BowSocket);
	}

}
