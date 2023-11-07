#include "OnAndOffImpactEffect_L.h"
#include "Global/Global.h"

#include "Weapon/Weapon.h"
#include "Interface/OwnerWeaponInterface.h"
#include "ActorComponents/C_PowerComponent.h"

void UOnAndOffImpactEffect_L::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{

	AActor* OwnerActor = MeshComp->GetOwner();
	CheckNull(OwnerActor);

	if (OwnerActor->GetClass()->ImplementsInterface(UOwnerWeaponInterface::StaticClass()))
	{
		IOwnerWeaponInterface* Interface = Cast<IOwnerWeaponInterface>(OwnerActor);
		AWeapon* CurrentWeapon = Interface->GetOwnerCurrentWeapon();
		UC_PowerComponent* WeaponPowerComp = CurrentWeapon->GetPowerComponent();

		CheckTrue(!Interface || !CurrentWeapon || !WeaponPowerComp);

		WeaponPowerComp->SetIsImpactEffectSocketNamesActive(0, true);
	}

}

void UOnAndOffImpactEffect_L::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{

	AActor* OwnerActor = MeshComp->GetOwner();
	CheckNull(OwnerActor);

	if (OwnerActor->GetClass()->ImplementsInterface(UOwnerWeaponInterface::StaticClass()))
	{
		IOwnerWeaponInterface* Interface = Cast<IOwnerWeaponInterface>(OwnerActor);
		AWeapon* CurrentWeapon = Interface->GetOwnerCurrentWeapon();
		UC_PowerComponent* WeaponPowerComp = CurrentWeapon->GetPowerComponent();

		CheckTrue(!Interface || !CurrentWeapon || !WeaponPowerComp);

		WeaponPowerComp->SetIsImpactEffectSocketNamesActive(0, false);
	}

}
