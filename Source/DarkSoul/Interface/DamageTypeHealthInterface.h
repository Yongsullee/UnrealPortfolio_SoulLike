#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Global/Global.h"

#include "DamageTypeHealthInterface.generated.h"

UINTERFACE(MinimalAPI)
class UDamageTypeHealthInterface : public UInterface
{
	GENERATED_BODY()
};

class DARKSOUL_API IDamageTypeHealthInterface
{
	GENERATED_BODY()

public:
	/*	********************************************************************************************************
		
		[Remark]	:	Add UFUNCTION() Reflection On Unreal Editor Makes Error

		목적:	
			"C_HealthComponent" 객체의 포인터를 "참조형"으로 인자를 넘겨 받아 "Out Parameter"로 활용하고자 합니다.
		설명:
			1. UFUNCTION()을 작성하면, Out Parameter로 받을 시 Log(Missing '*' in Expected a pointer type)가 발생합니다.
			2. UFUNCTION()을 지우니까, 에러가 발생하지 않는 특이한 점을 발견합니다.

	*	********************************************************************************************************/

	virtual void TriggerDebuffStatusEffect(class UC_HealthComponent* OutComp) = 0;
	virtual void TriggerBuffStatusEffect(class UC_PowerComponent* OutComp, const EAttributeType& InEnum) = 0;

	virtual float GetImpulseAmount() = 0;

};
