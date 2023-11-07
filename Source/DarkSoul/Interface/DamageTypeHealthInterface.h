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

		����:	
			"C_HealthComponent" ��ü�� �����͸� "������"���� ���ڸ� �Ѱ� �޾� "Out Parameter"�� Ȱ���ϰ��� �մϴ�.
		����:
			1. UFUNCTION()�� �ۼ��ϸ�, Out Parameter�� ���� �� Log(Missing '*' in Expected a pointer type)�� �߻��մϴ�.
			2. UFUNCTION()�� ����ϱ�, ������ �߻����� �ʴ� Ư���� ���� �߰��մϴ�.

	*	********************************************************************************************************/

	virtual void TriggerDebuffStatusEffect(class UC_HealthComponent* OutComp) = 0;
	virtual void TriggerBuffStatusEffect(class UC_PowerComponent* OutComp, const EAttributeType& InEnum) = 0;

	virtual float GetImpulseAmount() = 0;

};
