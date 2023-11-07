#include "BossEnemy.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

#include "ActorComponents/C_StateComponent.h"
#include "ActorComponents/C_WeaponComponent.h"

#include "AIController.h"
#include "BrainComponent.h"


ABossEnemy::ABossEnemy()
{

	PrimaryActorTick.bCanEverTick = false;

}

void ABossEnemy::BeginPlay()
{

	Super::BeginPlay();

}

void ABossEnemy::IsDead(AActor* InDamageCauser, float InImpulseAmount, bool bStatusEffectTriggered)
{
	
	if (InDamageCauser)
	{
		AActor* DamageCauserOwner = InDamageCauser->GetOwner();
		if (DamageCauserOwner)
		{
			ACharacter* DamageCauserOwnerCharacter = Cast<ACharacter>(InDamageCauser->GetOwner());
			if (DamageCauserOwnerCharacter)
			{
				OnEnemyIsDead.ExecuteIfBound(InDamageCauser, true, 0);
			}
		}

	}

	Super::IsDead(InDamageCauser, InImpulseAmount, bStatusEffectTriggered);

}