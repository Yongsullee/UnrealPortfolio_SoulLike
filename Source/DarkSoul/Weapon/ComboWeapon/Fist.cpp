#include "Fist.h"
#include "Global/Global.h"

#include "Components/SphereComponent.h"

#include "GameObjects/GameObject.h"

AFist::AFist()
{
	PrimaryActorTick.bCanEverTick = false;

	// Sphere Component
	CHelpers::CreateComponent<USceneComponent>(this, &Root, TEXT("Root Component"), RootComponent);

	CHelpers::CreateComponent<USphereComponent>(this, &LeftHand, TEXT("Left Hand"), Root);
	CHelpers::CreateComponent<USphereComponent>(this, &RightHand, TEXT("Right Hand"), Root);
	CHelpers::CreateComponent<USphereComponent>(this, &LeftFoot, TEXT("Left Foot"), Root);
	CHelpers::CreateComponent<USphereComponent>(this, &RightFoot, TEXT("Right Foot"), Root);

	SphereComponents.Reserve(SphereComponentsSize);
	SphereComponents.Add(LeftHand);
	SphereComponents.Add(RightHand);
	SphereComponents.Add(LeftFoot);
	SphereComponents.Add(RightFoot);

	for (auto sphere : SphereComponents)
	{
		sphere->SetSphereRadius(32.f);
		sphere->SetVisibility(false);
	}

	WeaponType = EWeaponType::E_Fist;
}

void AFist::BeginPlay()
{
	Super::BeginPlay();

	for (const auto sphere : SphereComponents)
	{
		sphere->OnComponentBeginOverlap.AddDynamic(this, &AFist::OnCapsuleBeginOverlap);
	}
}


void AFist::AttachComponentToOwnerComponent(USkeletalMeshComponent* InSkeletalMeshComponent)
{
	Super::AttachComponentToOwnerComponent(InSkeletalMeshComponent);

	LeftHand->AttachToComponent(InSkeletalMeshComponent, FAttachmentTransformRules::KeepRelativeTransform, TEXT("Fighter_LeftHand"));
	RightHand->AttachToComponent(InSkeletalMeshComponent, FAttachmentTransformRules::KeepRelativeTransform, TEXT("Fighter_RightHand"));
	LeftFoot->AttachToComponent(InSkeletalMeshComponent, FAttachmentTransformRules::KeepRelativeTransform, TEXT("Fighter_LeftFoot"));
	RightFoot->AttachToComponent(InSkeletalMeshComponent, FAttachmentTransformRules::KeepRelativeTransform, TEXT("Fighter_RightFoot"));
}

void AFist::OnCollision()
{
	Super::OnCollision();

	for (const auto sphere : SphereComponents)
	{
		sphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

void AFist::OffCollision()
{
	Super::OffCollision();

	for (const auto sphere : SphereComponents)
	{
		sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AFist::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	AActor* OwnerActor = GetOwner();
	AGameObject* OtherObject = Cast<AGameObject>(OtherActor);
	CheckTrue(!OtherActor || !OwnerActor || !OtherObject || OtherActor == OwnerActor);

	if (AttackHitActors.Contains(OtherActor) != true)
	{
		if (OnWeaponCollisionBeginOverlap.IsBound())
		{
			OnWeaponCollisionBeginOverlap.Execute(OwnerActor, OtherActor, IsSkillAttack, SkillBasePower);

			AttackHitActors.Add(OtherActor);
		}
	}

}
