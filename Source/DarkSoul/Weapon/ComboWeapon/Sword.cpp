#include "Sword.h"
#include "Global/Global.h"

#include "GameFramework/Character.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "ActorComponents/C_PowerComponent.h"

#include "GameObjects/GameObject.h"

ASword* ASword::Spawn(UWorld* InWorld, ACharacter* InOwner)
{
	FActorSpawnParameters params;

	params.Owner = InOwner;

	return InWorld->SpawnActor<ASword>(params);
}

ASword::ASword()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	SkeletalMesh->SetupAttachment(Root);

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(SkeletalMesh);
	Capsule->SetCapsuleHalfHeight(80.f);
	Capsule->SetCapsuleRadius(64.f);

	// Weapon Type
	WeaponType = EWeaponType::E_Sword;

	// Ghost Effect
	

}

void ASword::BeginPlay()
{

	Super::BeginPlay();

	Capsule->OnComponentBeginOverlap.AddDynamic(this, &ASword::OnCapsuleBeginOverlap);

}


void ASword::AttachComponentToOwnerComponent(USkeletalMeshComponent* InSkeletalMeshComponent)
{
	Super::AttachComponentToOwnerComponent(InSkeletalMeshComponent);

	Root->AttachToComponent(InSkeletalMeshComponent, FAttachmentTransformRules::KeepRelativeTransform, TEXT("Sword"));
}

void ASword::OnCollision()
{
	Super::OnCollision();

	Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ASword::OffCollision()
{
	Super::OffCollision();

	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASword::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckNull(OtherActor);


	AActor* OwnerActor = GetOwner();
	CheckNull(OwnerActor);


	AGameObject* OtherObject = Cast<AGameObject>(OtherActor);
	CheckNull(OtherObject);

	CheckTrue(OtherActor == OwnerActor);

	if (AttackHitActors.Contains(OtherActor) != true)
	{

		// Hit Effect Spawn
		

		if (OnWeaponCollisionBeginOverlap.IsBound())
		{
			OnWeaponCollisionBeginOverlap.Execute(OwnerActor, OtherActor, IsSkillAttack, SkillBasePower);

			AttackHitActors.Add(OtherActor);
		}

	}
}

