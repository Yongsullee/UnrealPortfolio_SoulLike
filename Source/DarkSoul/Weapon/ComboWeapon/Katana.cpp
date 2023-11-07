#include "Katana.h"
#include "Global/Global.h"

#include "GameFramework/Character.h"

#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"

#include "ActorComponents/C_PowerComponent.h"

#include "GameObjects/GameObject.h"

AKatana::AKatana()
{

	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Static Mesh"));
	SkeletalMesh->SetupAttachment(Root);

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(SkeletalMesh);
	Capsule->SetCapsuleHalfHeight(100.f);
	Capsule->SetCapsuleRadius(80.f);

	CHelpers::CreateComponent<USphereComponent>(this, &RightFoot, TEXT("Right Foot"), Root);
	RightFoot->SetSphereRadius(64.f);

	WeaponType = EWeaponType::E_Katana;

}

void AKatana::BeginPlay()
{
	Super::BeginPlay();

	Capsule->OnComponentBeginOverlap.AddDynamic(this, &AKatana::OnCapsuleBeginOverlap);
	RightFoot->OnComponentBeginOverlap.AddDynamic(this, &AKatana::OnCapsuleBeginOverlap);

}

void AKatana::AttachComponentToOwnerComponent(USkeletalMeshComponent* InSkeletalMeshComponent)
{
	Super::AttachComponentToOwnerComponent(InSkeletalMeshComponent);

	SkeletalMesh->AttachToComponent(InSkeletalMeshComponent, FAttachmentTransformRules::KeepRelativeTransform, TEXT("Katana"));
	RightFoot->AttachToComponent(InSkeletalMeshComponent, FAttachmentTransformRules::KeepRelativeTransform, TEXT("Fighter_RightFoot"));
}

void AKatana::OnCollision()
{
	Super::OnCollision();

	Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RightFoot->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AKatana::OffCollision()
{
	Super::OffCollision();

	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightFoot->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AKatana::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckNull(OtherActor);

	AActor* OwnerActor = GetOwner();
	CheckNull(OwnerActor);

	AGameObject* OtherObject = Cast<AGameObject>(OtherActor);
	CheckNull(OtherObject);

	CheckTrue(OtherActor == OwnerActor);

	if (AttackHitActors.Contains(OtherActor) != true)
	{
		if (OnWeaponCollisionBeginOverlap.IsBound())
		{
			OnWeaponCollisionBeginOverlap.Execute(OwnerActor, OtherActor, IsSkillAttack, SkillBasePower);

			AttackHitActors.Add(OtherActor);
		}
	}
}
