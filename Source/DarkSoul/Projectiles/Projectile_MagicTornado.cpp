#include "Projectile_MagicTornado.h"
#include "Global/Global.h"

#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AProjectile_MagicTornado::AProjectile_MagicTornado()
{
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &TornadoParitcle, TEXT("Tornado Particle"), Sphere);

	RootComponent = Sphere;
	Sphere->InitSphereRadius(50.f);
	Sphere->SetVisibility(false);
	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Scene->SetupAttachment(Sphere);

	// Box Collision
	BoxCollision->SetupAttachment(Sphere);
	BoxCollision->SetRelativeLocation(FVector(0.f, 0.f, 338.f));
	BoxCollision->SetBoxExtent(FVector(250.f, 250.f, 400.f));
	BoxCollision->SetVisibility(false);
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	TornadoParitcle->bAutoActivate = false;

	CheckNull(ProjectileMovement);
	ProjectileMovement->ProjectileGravityScale = 0.f;
	ProjectileMovement->InitialSpeed = 2000.f;
	ProjectileMovement->MaxSpeed = 5000.f;

	ProjectileType = EProjectileType::E_Tornado;

}

void AProjectile_MagicTornado::BeginPlay()
{

	Super::BeginPlay();

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AProjectile_MagicTornado::OnCapsuleBeginOverlap);

}

void AProjectile_MagicTornado::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectile_MagicTornado::OnCollision()
{
	Super::OnCollision();

	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AProjectile_MagicTornado::OffCollision()
{
	Super::OnCollision();

	BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

/**********************************************************************************

	����:
		0) Sphere		Collision On
		1) Tornado		Activate

***********************************************************************************/

void AProjectile_MagicTornado::StartLaunched()
{
	
	Super::StartLaunched();

	CheckTrue(BoxCollision == nullptr || TornadoParitcle == nullptr);

	// #0. Collision
	OnCollision();
	// #1. Effect On
	if(TornadoParitcle->IsActive() == false) TornadoParitcle->Activate();

}

/*	**************************************************************************************************************
*
*	[Remark]	: ���� Magic Projectile�� �޸�, ������ �߻�ü�μ� �浹 �� �ٷ� Effect�� Deactivate ���� �ʽ��ϴ�.
*
*	����:
*		�� ��ü�� �浹�� �߻��ص�, Effect�� Deactive ���� �ʽ��ϴ�.
*
*	����:
*		1. �浹 �߻��� ���� ���� ���� Other Actor�� nullptr�� �ƴմϴ�.
*		2. Expire Handler�� ���� ���� ���� Other Actor�� nullptr �Դϴ�.
*		3. ����, Other Actor�� Nullptr�� ����, Projectile Effect�� Deactivate �մϴ�.
*
*	**************************************************************************************************************/

void AProjectile_MagicTornado::EndLaunched()
{

	Super::EndLaunched();

	CheckTrue(BoxCollision == nullptr || TornadoParitcle == nullptr);

	// #0, Collision
	OffCollision();
	// #1. Effect Off 
	if (TornadoParitcle->IsActive() == true) TornadoParitcle->DeactivateImmediate();

}

void AProjectile_MagicTornado::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	Super::OnCapsuleBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

}
