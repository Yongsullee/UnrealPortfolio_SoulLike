#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"

#include "TP_WeaponSpawnLocation.generated.h"

UCLASS()
class DARKSOUL_API ATP_WeaponSpawnLocation : public ATargetPoint
{
	GENERATED_BODY()
	
private:

	UPROPERTY(EditAnywhere, Category = "Weapon")
		TSubclassOf<class AWeapon> WeaponClass;

public:

	FORCEINLINE TSubclassOf<class AWeapon> GetWeaponClass() { return WeaponClass; }
};
