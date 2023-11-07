#include "OnAndOffImpactEffect_R.h"
#include "Global/Global.h"

#include "Weapon/Weapon.h"
#include "Interface/OwnerWeaponInterface.h"
#include "ActorComponents/C_PowerComponent.h"

void UOnAndOffImpactEffect_R::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{

	AActor* PlayerActor = MeshComp->GetOwner();
	CheckNull(PlayerActor);

	if (PlayerActor->GetClass()->ImplementsInterface(UOwnerWeaponInterface::StaticClass()))
	{

		IOwnerWeaponInterface* Interface = Cast<IOwnerWeaponInterface>(PlayerActor);
		CheckNull(Interface);

		AWeapon* CurrentWeapon = Interface->GetOwnerCurrentWeapon();
		CheckNull(CurrentWeapon);

		CurrentWeapon->GetPowerComponent()->SetIsImpactEffectSocketNamesActive(1, true);

	}

}

void UOnAndOffImpactEffect_R::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

	AActor* PlayerActor = MeshComp->GetOwner();
	CheckNull(PlayerActor);

	if (PlayerActor->GetClass()->ImplementsInterface(UOwnerWeaponInterface::StaticClass()))
	{

		IOwnerWeaponInterface* Interface = Cast<IOwnerWeaponInterface>(PlayerActor);
		CheckNull(Interface);

		AWeapon* CurrentWeapon = Interface->GetOwnerCurrentWeapon();
		CheckNull(CurrentWeapon);

		CurrentWeapon->GetPowerComponent()->SetIsImpactEffectSocketNamesActive(1, false);

	}

}
