#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Global/Global.h"
#include "Weapon/Weapon.h"

#include "OwnerWeaponInterface.generated.h"

UINTERFACE(MinimalAPI)
class UOwnerWeaponInterface : public UInterface
{
	GENERATED_BODY()
};

class DARKSOUL_API IOwnerWeaponInterface
{
	GENERATED_BODY()

public:
	virtual EStateType GetOwnerStateType() = 0;
	virtual void SetOwnerStateType(EStateType StateTypeVal) = 0;

	virtual EComboType GetOwnerComboType() = 0;

	virtual AWeapon* GetOwnerCurrentWeapon() = 0;

};
