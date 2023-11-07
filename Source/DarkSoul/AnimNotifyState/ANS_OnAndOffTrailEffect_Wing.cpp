#include "ANS_OnAndOffTrailEffect_Wing.h"
#include "Global/Global.h"

#include "Weapon/Weapon.h"
#include "Interface/OwnerWeaponInterface.h"
#include "ActorComponents/C_PowerComponent.h"

#include "NiagaraFunctionLibrary.h"

void UANS_OnAndOffTrailEffect_Wing::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{

	AActor* OwnerActor = MeshComp->GetOwner();
	CheckNull(OwnerActor);

	if (OwnerActor->GetClass()->ImplementsInterface(UOwnerWeaponInterface::StaticClass()))
	{
		IOwnerWeaponInterface* Interface = Cast<IOwnerWeaponInterface>(OwnerActor);
		AWeapon* CurrentWeapon = Interface->GetOwnerCurrentWeapon();
		UNiagaraSystem* TrailEffect = CurrentWeapon->GetPowerComponent()->GetPowerInfoStruct().TrailEffect;

		CheckTrue(!Interface || !CurrentWeapon || !TrailEffect);

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			OwnerActor->GetWorld(),
			CurrentWeapon->GetPowerComponent()->GetPowerInfoStruct().TrailEffect,
			MeshComp->GetSocketLocation(EffectSocket),
			MeshComp->GetSocketRotation(EffectSocket)
		);
	}

}
