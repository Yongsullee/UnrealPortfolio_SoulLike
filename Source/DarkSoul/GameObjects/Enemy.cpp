#include "Enemy.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

#include "ActorComponents/C_StateComponent.h"
#include "ActorComponents/C_WeaponComponent.h"

#include "AIController.h"
#include "BrainComponent.h"

AEnemy::AEnemy()
{

	PrimaryActorTick.bCanEverTick = false;

}

void AEnemy::BeginPlay()
{

	Super::BeginPlay();

}

void AEnemy::IsDead(AActor* InDamageCauser, float InImpulseAmount, bool bStatusEffectTriggered)
{

	if (InDamageCauser)
	{
		AActor* DamageCauserOwner = InDamageCauser->GetOwner();
		if (DamageCauserOwner)
		{
			ACharacter* DamageCauserOwnerCharacter = Cast<ACharacter>(InDamageCauser->GetOwner());
			if (DamageCauserOwnerCharacter)
			{
				OnEnemyIsDead.ExecuteIfBound(InDamageCauser, false, PoolObjectInfo_Struct.Index);
			}
		}

	}


	Super::IsDead(InDamageCauser, InImpulseAmount, bStatusEffectTriggered);

}