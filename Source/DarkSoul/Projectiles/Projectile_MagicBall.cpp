#include "Projectile_MagicBall.h"
#include "Global/Global.h"

#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AProjectile_MagicBall::AProjectile_MagicBall()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	MuzzleEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Muzzle Effect"));
	ProjectileEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Projectile Effect"));
	HitEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Hit Effect"));

	RootComponent = Sphere;
	Sphere->InitSphereRadius(64.f);
	Sphere->SetVisibility(false);
	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Scene->SetupAttachment(Sphere);

	MuzzleEffect->SetupAttachment(Sphere);
	MuzzleEffect->bAutoActivate = false;

	ProjectileEffect->SetupAttachment(Sphere);
	ProjectileEffect->bAutoActivate = false;

	HitEffect->SetupAttachment(Sphere);
	HitEffect->bAutoActivate = false;

	CheckNull(ProjectileMovement);
	ProjectileMovement->InitialSpeed = 2000.f;
	ProjectileMovement->MaxSpeed = 5000.f;

	// Life Span
	OffMuzzleEffectRate = 0.5f;
	OffHitEffectRate = 0.5f;

	ProjectileType = EProjectileType::E_MagicBall;

}

void AProjectile_MagicBall::BeginPlay()
{

	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AProjectile_MagicBall::OnCapsuleBeginOverlap);

}

void AProjectile_MagicBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectile_MagicBall::OnCollision()
{
	// Empty
	Super::OnCollision();

	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

}

void AProjectile_MagicBall::OffCollision()
{
	// Empty
	Super::OffCollision();

	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

/**********************************************************************************
	설명:
		1) Muzzle		Effect Off
		2) Projectile	Effect On
		3) Collision	On

***********************************************************************************/

void AProjectile_MagicBall::StartLaunched()
{

	Super::StartLaunched();

	CheckTrue(Sphere == nullptr || MuzzleEffect == nullptr || ProjectileEffect == nullptr);

	// #0. Collision
	OnCollision();

	// #2 Effects
	MuzzleEffect->Activate();
	ProjectileEffect->Activate();

}

/**********************************************************************************
	
	설명:
		1) Projectile	Effect Off
		2) Hit			Effect On
		3) Collision	Off
		4) Destroy

***********************************************************************************/

void AProjectile_MagicBall::EndLaunched()
{

	Super::EndLaunched();

	CheckTrue(Sphere == nullptr || MuzzleEffect == nullptr || ProjectileEffect == nullptr || HitEffect == nullptr);

	// #0. Collision
	OffCollision();

	// #1. Effects
	MuzzleEffect->DeactivateImmediate();
	ProjectileEffect->DeactivateImmediate();
	HitEffect->Activate();

}

void AProjectile_MagicBall::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	Super::OnCapsuleBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

}
