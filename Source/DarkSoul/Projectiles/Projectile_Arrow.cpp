#include "Projectile_Arrow.h"
#include "Global/Global.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "NiagaraComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "GameObjects/GameObject.h"


AProjectile_Arrow::AProjectile_Arrow()
{

	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent(this, &Root, TEXT("Root"));
	CHelpers::CreateComponent(this, &StaticMesh, TEXT("StaticMesh"), Root);
	CHelpers::CreateComponent(this, &Arrow, TEXT("Aroow"), StaticMesh);
	CHelpers::CreateComponent(this, &ImpactEffect, TEXT("Impact Effect"), Root);
	CHelpers::CreateComponent(this, &Sphere, TEXT("Sphere"), StaticMesh);

	RootComponent = Root;

	StaticMesh->SetRelativeScale3D(FVector(1.5f, 1.f, 1.f));
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Sphere->InitSphereRadius(64.f);
	Sphere->SetRelativeLocation(FVector(75.f, 0.f, 0.f));
	Sphere->SetVisibility(false);
	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ImpactEffect->SetRelativeLocation(FVector(120.f, 0.f, 0.f));
	ImpactEffect->bAutoActivate = false;

	ProjectileType = EProjectileType::E_Arrow;

}

void AProjectile_Arrow::BeginPlay()
{

	Super::BeginPlay();

	// #0 Dynamic Skeletal Mesh Asset
	UStaticMesh* Mesh;
	CHelpers::GetAssetDynamic<UStaticMesh>(&Mesh, "StaticMesh'/Game/DynamicCombatSystem/Meshes/SM_ElvenArrow.SM_ElvenArrow'");
	CheckNull(Mesh);
	StaticMesh->SetStaticMesh(Mesh);

	UNiagaraSystem* Effect;
	CHelpers::GetAssetDynamic<UNiagaraSystem>(&Effect, "NiagaraSystem'/Game/Hack_And_Slash_FX/VFX_Niagara/Impacts/NS_Fire_Slash_Impact.NS_Fire_Slash_Impact'");
	CheckNull(Effect);
	ImpactEffect->SetAsset(Effect);

	// #2 Collision Delegate
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AProjectile_Arrow::OnCapsuleBeginOverlap);

}

void AProjectile_Arrow::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);

}

void AProjectile_Arrow::OnCollision()
{

	Super::OnCollision();

	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

}

void AProjectile_Arrow::OffCollision()
{

	Super::OffCollision();

	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}


void AProjectile_Arrow::StartLaunched()
{

	Super::StartLaunched();

	AActor* OwnerWeapon = GetOwner();

	CheckTrue(OwnerWeapon == nullptr || ProjectileMovement == nullptr || Sphere == nullptr);

	// #0. Projectile Movement
	ProjectileMovement->SetVelocityInLocalSpace(FVector(1.f, 0.f, 0.f) * ProjectileMovement->InitialSpeed);

	// #0 Sphere Collision
	OnCollision();

}


void AProjectile_Arrow::EndLaunched() 
{

	Super::EndLaunched();

	CheckTrue(Sphere == nullptr || ImpactEffect == nullptr);

	// #3 Collision Off
	OffCollision();

	// #2 Impact Effect Activate
	ImpactEffect->Activate();	

}

void AProjectile_Arrow::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	CheckTrue(!OtherActor);

	AActor* OwnerWeapon = GetOwner();
	AGameObject* OwnerObject = Cast<AGameObject>(OwnerWeapon->GetOwner());
	AGameObject* OtherObject = Cast<AGameObject>(OtherActor);

	CheckTrue(!OtherObject || !OwnerWeapon || !OwnerObject || OtherActor == OwnerWeapon || OtherActor == OwnerObject || OwnerObject->IsHostile(OtherObject) == false);

	// #0. Projectile::OnCapsuleBeginOverlap
	Super::OnCapsuleBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	// #1 End Launched 
	if(bPenetrable == false) EndLaunched();

}
