#include "C_ClassComponent.h"
#include "Global/Global.h"

UC_ClassComponent::UC_ClassComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UC_ClassComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UC_ClassComponent::ChangeCombatType(EPlayerCombatType InType)
{
	if (CombatType == InType)
		CombatType = EPlayerCombatType::E_Unarmed;
	else
		CombatType = InType;

}
