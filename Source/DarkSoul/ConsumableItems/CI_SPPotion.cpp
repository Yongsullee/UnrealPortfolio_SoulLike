#include "CI_SPPotion.h"
#include "Global/Global.h"

#include "GameObjects/PlayerCharacter.h"
#include "ActorComponents/C_StaminaComponent.h"

#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

ACI_SPPotion::ACI_SPPotion()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACI_SPPotion::BeginPlay()
{
	Super::BeginPlay();

}

void ACI_SPPotion::StartActivation(APlayerCharacter** OutPlayer)
{

	Super::StartActivation(OutPlayer);

	(*OutPlayer)->GetStaminaComponent()->RecoverCurrentStamina(SPRecoveryAmount);

	CheckNull(Effect);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Effect, (*OutPlayer)->GetActorLocation() - FVector(0.f, 0.f, 100.f));

}
