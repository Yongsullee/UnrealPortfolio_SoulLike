#include "OnAndOffZoneEffect.h"
#include "Global/Global.h"

#include "Weapon/Weapon.h"
#include "Interface/OwnerWeaponInterface.h"

void UOnAndOffZoneEffect::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{

	AActor* PlayerActor = MeshComp->GetOwner();
	CheckNull(PlayerActor);

	if (PlayerActor->GetClass()->ImplementsInterface(UOwnerWeaponInterface::StaticClass()))
	{
		IOwnerWeaponInterface* Interface = Cast<IOwnerWeaponInterface>(PlayerActor);
		AWeapon* CurrentWeapon = Interface->GetOwnerCurrentWeapon();

		CheckTrue(!Interface || !CurrentWeapon);

		CurrentWeapon->OnZoneEffect();
	}

}

void UOnAndOffZoneEffect::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

	AActor* PlayerActor = MeshComp->GetOwner();
	CheckNull(PlayerActor);

	if (PlayerActor->GetClass()->ImplementsInterface(UOwnerWeaponInterface::StaticClass()))
	{
		IOwnerWeaponInterface* Interface = Cast<IOwnerWeaponInterface>(PlayerActor);
		AWeapon* CurrentWeapon = Interface->GetOwnerCurrentWeapon();

		CheckTrue(!Interface || !CurrentWeapon);

		CurrentWeapon->OffZoneEffect();
	}

}
