#include "StatusEffect.h"
#include "Global/Global.h"

#include "Components/SceneComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/TextRenderComponent.h"

AStatusEffect::AStatusEffect()
{

	// Root
	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	//Particle
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &Particle, "Particle", Root);
	Particle->bAutoActivate = false;

}

void AStatusEffect::BeginPlay()
{
	Super::BeginPlay();
}

void AStatusEffect::StartActivation()
{

	if(!!Particle)
		Particle->Activate();

}

void AStatusEffect::EndActivation()
{

	if(!!Particle && Particle->IsActive())
		Particle->Deactivate();

}

void AStatusEffect::AttachComponentToOwnerComponent(USkeletalMeshComponent* InSkeletalMeshComponent)
{
	Root->AttachToComponent(InSkeletalMeshComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
}