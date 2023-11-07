#include "CI_Potion.h"
#include "Global/Global.h"

#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"

#include "GameObjects/PlayerCharacter.h"


ACI_Potion::ACI_Potion()
{
	PrimaryActorTick.bCanEverTick = false;

	CHelpers::CreateComponent<USceneComponent>(this, &Root, TEXT("Root"));
	RootComponent = Root;

	Potion = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Potion"));
	Potion->SetupAttachment(Root);
	Potion->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.5f));
	Potion->bAutoActivate = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(Root);
	Sphere->InitSphereRadius(85.f);
	Sphere->SetVisibility(false);
	Sphere->SetRelativeLocation(FVector(0.f, 0.f, 35.f));
	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void ACI_Potion::BeginPlay()
{
	Super::BeginPlay();

	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}