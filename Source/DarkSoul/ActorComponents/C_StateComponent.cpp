#include "C_StateComponent.h"
#include "Global/Global.h"

#include "GameObjects/GameObject.h"
#include "AIController/AIC_EnemyBase.h"

UC_StateComponent::UC_StateComponent()
{

	PrimaryComponentTick.bCanEverTick = false;

}


void UC_StateComponent::BeginPlay()
{

	Super::BeginPlay();

	AGameObject* OwnerObject = Cast<AGameObject>(GetOwner());
	CheckTrue(!OwnerObject);

	OwnerObject->OnChangeStateType.BindUFunction(this, "SetStateType");

	AAIC_EnemyBase* OwnerAIController = Cast<AAIC_EnemyBase>(OwnerObject->GetController());
	CheckTrue(!OwnerAIController);

	OwnerAIController->OnChangeAgentAIState.BindUFunction(this, "SetAIStateType");

}