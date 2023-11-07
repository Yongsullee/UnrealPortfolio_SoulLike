#include "CastleKnight_Axe.h"
#include "Global/Global.h"

#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "ActorComponents/C_PowerComponent.h"

#include "GameObjects/EnemyBase.h"


ACastleKnight_Axe* ACastleKnight_Axe::Spawn(UWorld* InWorld, ACharacter* InOwner)
{
	FActorSpawnParameters params;

	params.Owner = InOwner;

	return InWorld->SpawnActor<ACastleKnight_Axe>(params);
}

ACastleKnight_Axe::ACastleKnight_Axe()
{

	PrimaryActorTick.bCanEverTick = false;

	CHelpers::CreateComponent(this, &Root, TEXT("Root"));
	CHelpers::CreateComponent(this, &StaticMesh, TEXT("StaticMesh"), Root);
	CHelpers::CreateComponent(this, &Capsule, TEXT("Capsule"), StaticMesh);

	StaticMesh->SetRelativeScale3D(FVector(1.25f, 1.25f, 1.25f));


	WeaponType = EWeaponType::E_CastleKnight_Axe;
}

void ACastleKnight_Axe::BeginPlay()
{

	Super::BeginPlay();

	Capsule->OnComponentBeginOverlap.AddDynamic(this, &ACastleKnight_Axe::OnCapsuleBeginOverlap);

}


void ACastleKnight_Axe::AttachComponentToOwnerComponent(USkeletalMeshComponent* InSkeletalMeshComponent)
{

	Super::AttachComponentToOwnerComponent(InSkeletalMeshComponent);

	Root->AttachToComponent(InSkeletalMeshComponent, FAttachmentTransformRules::KeepRelativeTransform, TEXT("CastleKnight_Axe"));

}

void ACastleKnight_Axe::OnCollision()
{

	Super::OnCollision();

	Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

}

void ACastleKnight_Axe::OffCollision()
{

	Super::OffCollision();

	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}


void ACastleKnight_Axe::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	CheckNull(OtherActor);

	AActor* OwnerActor = GetOwner();
	CheckNull(OwnerActor);

	AGameObject* OtherObject = Cast<AGameObject>(OtherActor);
	CheckNull(OtherObject);

	CheckTrue(OtherActor == OwnerActor);

	CheckFalse(Cast<AEnemyBase>(OtherActor) == nullptr);

	if (AttackHitActors.Contains(OtherActor) != true)
	{
		if (OnWeaponCollisionBeginOverlap.IsBound())
		{
			OnWeaponCollisionBeginOverlap.Execute(OwnerActor, OtherActor, IsSkillAttack, SkillBasePower);

			AttackHitActors.Add(OtherActor);
		}
	}

}
