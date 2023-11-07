#include "SwordAndShield.h"
#include "Global/Global.h"

#include "GameFramework/Character.h"

#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "GameObjects/PlayerCharacter.h"
#include "ActorComponents/C_PowerComponent.h"


ASwordAndShield::ASwordAndShield()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SkeletalMesh_Sword = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Sword"));
	Capsule_Sword = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule_Sword"));
	SkeletalMesh_Sheild = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Shield"));
	Capsule_Shield = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule_Shield"));
	CHelpers::CreateComponent<USphereComponent>(this, &RightFoot, TEXT("Right Foot"), Root);

	SetRootComponent(Root);
	SkeletalMesh_Sword->SetupAttachment(Root);
	Capsule_Sword->SetupAttachment(SkeletalMesh_Sword);
	SkeletalMesh_Sheild->SetupAttachment(Root);
	Capsule_Shield->SetupAttachment(SkeletalMesh_Sheild);
	RightFoot->SetSphereRadius(32.f);

	WeaponType = EWeaponType::E_SwordAndShield;
}

void ASwordAndShield::BeginPlay()
{
	Super::BeginPlay();

	Capsule_Sword->OnComponentBeginOverlap.AddDynamic(this, &ASwordAndShield::OnCapsuleBeginOverlap);
	Capsule_Shield->OnComponentBeginOverlap.AddDynamic(this, &ASwordAndShield::OnCapsuleBeginOverlap);
	RightFoot->OnComponentBeginOverlap.AddDynamic(this, &ASwordAndShield::OnCapsuleBeginOverlap);
}


void ASwordAndShield::AttachComponentToOwnerComponent(USkeletalMeshComponent* InSkeletalMeshComponent)
{

	Super::AttachComponentToOwnerComponent(InSkeletalMeshComponent);

	RightFoot->AttachToComponent(InSkeletalMeshComponent, FAttachmentTransformRules::KeepRelativeTransform, TEXT("Fighter_RightFoot"));

	SkeletalMesh_Sword->AttachToComponent(InSkeletalMeshComponent, FAttachmentTransformRules::KeepRelativeTransform, TEXT("SwordAndShield_Sword"));
	SkeletalMesh_Sheild->AttachToComponent(InSkeletalMeshComponent, FAttachmentTransformRules::KeepRelativeTransform, TEXT("SwordAndShield_Shield"));

}

void ASwordAndShield::OnCollision()
{
	Super::OnCollision();

	Capsule_Sword->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Capsule_Shield->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RightFoot->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ASwordAndShield::OffCollision()
{

	Super::OffCollision();

	Capsule_Sword->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Capsule_Shield->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightFoot->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void ASwordAndShield::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckNull(OtherActor);

	AGameObject* OtherObject = Cast<AGameObject>(OtherActor);
	CheckNull(OtherObject);

	AActor* OwnerActor = GetOwner();
	CheckNull(OwnerActor);

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
