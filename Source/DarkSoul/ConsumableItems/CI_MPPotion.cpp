#include "CI_MPPotion.h"
#include "Global/Global.h"

#include "GameObjects/PlayerCharacter.h"
#include "ActorComponents/C_ManaComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"


ACI_MPPotion::ACI_MPPotion()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACI_MPPotion::BeginPlay()
{
	Super::BeginPlay();

}

void ACI_MPPotion::StartActivation(APlayerCharacter** OutPlayer)
{

	Super::StartActivation(OutPlayer);

	(*OutPlayer)->GetManaComponent()->RecoverLeftMana(MPRecoveryAmount);

	CheckNull(Effect);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Effect, (*OutPlayer)->GetActorLocation() - FVector(0.f, 0.f, 100.f));

}
