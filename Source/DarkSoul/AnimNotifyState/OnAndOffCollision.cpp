#include "OnAndOffCollision.h"
#include "Global/Global.h"

#include "Weapon/Weapon.h"
#include "Interface/OwnerWeaponInterface.h"

void UOnAndOffCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{

	AActor* PlayerActor = MeshComp->GetOwner();
	CheckNull(PlayerActor);

	if (PlayerActor->GetClass()->ImplementsInterface(UOwnerWeaponInterface::StaticClass()))
	{

		IOwnerWeaponInterface* Interface = Cast<IOwnerWeaponInterface>(PlayerActor);
		CheckNull(Interface);
		
		AWeapon* CurrentWeapon = Interface->GetOwnerCurrentWeapon();
		CheckNull(CurrentWeapon);
		
		CurrentWeapon->OnCollision();

	}

}

void UOnAndOffCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AActor* PlayerActor = MeshComp->GetOwner();
	CheckNull(PlayerActor);

	if (PlayerActor->GetClass()->ImplementsInterface(UOwnerWeaponInterface::StaticClass()))
	{
		IOwnerWeaponInterface* Interface = Cast<IOwnerWeaponInterface>(PlayerActor);
		CheckNull(Interface);

		AWeapon* CurrentWeapon = Interface->GetOwnerCurrentWeapon();
		CheckNull(CurrentWeapon);

		CurrentWeapon->OffCollision();

	}
}