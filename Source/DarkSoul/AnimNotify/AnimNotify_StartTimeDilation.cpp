#include "AnimNotify_StartTimeDilation.h"

#include "Interface/OwnerWeaponInterface.h"
#include "Weapon/ComboWeapon/ComboWeapon.h"

void UAnimNotify_StartTimeDilation::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* PlayerActor = MeshComp->GetOwner();
	CheckNull(PlayerActor);

	if (PlayerActor->GetClass()->ImplementsInterface(UOwnerWeaponInterface::StaticClass()))
	{

		IOwnerWeaponInterface* Interface = Cast<IOwnerWeaponInterface>(PlayerActor);
		CheckNull(Interface);

		AComboWeapon* CurrentWeapon = Cast<AComboWeapon>(Interface->GetOwnerCurrentWeapon());
		CheckNull(CurrentWeapon);

		CurrentWeapon->StartAttackDilation();

	}
}
