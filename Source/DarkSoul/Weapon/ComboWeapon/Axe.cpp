#include "Axe.h"
#include "Global/Global.h"

#include "GameFramework/Character.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"

#include "ActorComponents/C_PowerComponent.h"

#include "GameObjects/GameObject.h"

AAxe* AAxe::Spawn(UWorld* InWorld, ACharacter* InOwner)
{
	FActorSpawnParameters params;

	params.Owner = InOwner;

	return InWorld->SpawnActor<AAxe>(params);
}

AAxe::AAxe()
{

	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));

	SetRootComponent(Root);
	SkeletalMesh->SetupAttachment(Root);
	Capsule->SetupAttachment(SkeletalMesh);

	Capsule->SetCapsuleHalfHeight(80.f);
	Capsule->SetCapsuleRadius(60.f);

	WeaponType = EWeaponType::E_Axe;

}

void AAxe::BeginPlay()
{

	Super::BeginPlay();

	Capsule->OnComponentBeginOverlap.AddDynamic(this, &AAxe::OnCapsuleBeginOverlap);

}


void AAxe::AttachComponentToOwnerComponent(USkeletalMeshComponent* InSkeletalMeshComponent)
{
	Super::AttachComponentToOwnerComponent(InSkeletalMeshComponent);

	Root->AttachToComponent(InSkeletalMeshComponent, FAttachmentTransformRules::KeepRelativeTransform, TEXT("Axe"));
}

void AAxe::OnCollision()
{
	Super::OnCollision();

	Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AAxe::OffCollision()
{
	Super::OffCollision();

	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AAxe::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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
