#include "Projectile_MagicSkullBall.h"
#include "Global/Global.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"

AProjectile_MagicSkullBall::AProjectile_MagicSkullBall()
{
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	RootComponent = Sphere;
	Sphere->InitSphereRadius(64.f);
	Sphere->SetVisibility(false);
	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	Scene->SetupAttachment(Sphere);
	MuzzleEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Muzzle Effect"));
	MuzzleEffect->SetupAttachment(Sphere);
	MuzzleEffect->bAutoActivate = true;

	ProjectileEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Projectile Effect"));
	ProjectileEffect->SetupAttachment(Sphere);
	ProjectileEffect->bAutoActivate = false;

	HitEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Hit Effect"));
	HitEffect->SetupAttachment(Sphere);
	HitEffect->bAutoActivate = false;

	CheckNull(ProjectileMovement);
	ProjectileMovement->InitialSpeed = 2000.f;
	ProjectileMovement->MaxSpeed = 5000.f;

	// Timer Setting
	OffMuzzleEffectRate = 0.5f;
	OffHitEffectRate = 0.5f;

	ProjectileType = EProjectileType::E_SkullBall;

}

void AProjectile_MagicSkullBall::BeginPlay()
{

	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AProjectile_MagicSkullBall::OnCapsuleBeginOverlap);

}

void AProjectile_MagicSkullBall::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);

}

void AProjectile_MagicSkullBall::OnCollision()
{

	Super::OnCollision();

	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

}

void AProjectile_MagicSkullBall::OffCollision()
{

	Super::OffCollision();

	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}


void AProjectile_MagicSkullBall::StartLaunched()
{

	Super::StartLaunched();

	CheckTrue(Sphere == nullptr || MuzzleEffect == nullptr || ProjectileEffect == nullptr);

	// #0. Sphere Collision
	OnCollision();

	// #1. Effect
	MuzzleEffect->Activate();
	ProjectileEffect->Activate();

}


void AProjectile_MagicSkullBall::EndLaunched()
{

	Super::EndLaunched();
	
	CheckTrue(Sphere == nullptr || ProjectileEffect == nullptr || HitEffect == nullptr);

	// #0. Sphere Collision
	OffCollision();

	// #1. Effect
	MuzzleEffect->DeactivateImmediate();
	ProjectileEffect->DeactivateImmediate();
	HitEffect->Activate();
	
}

void AProjectile_MagicSkullBall::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	Super::OnCapsuleBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

}
