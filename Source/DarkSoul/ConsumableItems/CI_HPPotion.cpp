#include "CI_HPPotion.h"
#include "Global/Global.h"

#include "GameObjects/PlayerCharacter.h"
#include "ActorComponents/C_HealthComponent.h"

#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"


ACI_HPPotion::ACI_HPPotion()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACI_HPPotion::BeginPlay()
{
	Super::BeginPlay();
}

void ACI_HPPotion::StartActivation(APlayerCharacter** OutPlayer)
{
	
	Super::StartActivation(OutPlayer);

	(*OutPlayer)->GetHealthComponent()->RecoverCurrentHealth(HPRecoveryAmount);

	CheckNull(Effect);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Effect, (*OutPlayer)->GetActorLocation() - FVector(0.f, 0.f, 100.f));

}