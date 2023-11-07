#include "Projectile_MagicRay.h"
#include "Global/Global.h"

#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"

#include "Weapon/Weapon.h"
#include "Weapon/NoComboWeapon/Magic.h"
#include "GameFramework/ProjectileMovementComponent.h"

AProjectile_MagicRay::AProjectile_MagicRay()
{
	
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	MuzzleEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Muzzle Effect"));
	ProjectileEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Projectile Effect"));

	RootComponent = Sphere;
	Sphere->InitSphereRadius(50.f);
	Sphere->SetVisibility(false);
	Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Scene->SetupAttachment(Sphere);

	ProjectileEffect->SetupAttachment(Sphere);
	ProjectileEffect->bAutoActivate = false;

	MuzzleEffect->SetupAttachment(Sphere);
	MuzzleEffect->bAutoActivate = false;

	CheckTrue(ProjectileMovement == nullptr);
	ProjectileMovement->InitialSpeed = 0.f;
	ProjectileMovement->MaxSpeed = 0.f;

	// Expire Handler
	bPenetrable = true;
	ExpireRate = 1.5f;

	// Projectile Type
	ProjectileType = EProjectileType::E_Ray;

}

void AProjectile_MagicRay::BeginPlay()
{

	Super::BeginPlay();

}

void AProjectile_MagicRay::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);

	if(ProjectileEffect->IsActive()) BoxTracing();

}

void AProjectile_MagicRay::OnCollision()
{

	Super::OnCollision();

}

void AProjectile_MagicRay::OffCollision()
{

	Super::OffCollision();

}

/**********************************************************************************
	����:
		0) Sphere		Collision On
		1) Muzzle		Effect Off 
		2) Projectile	Effect On

***********************************************************************************/

void AProjectile_MagicRay::StartLaunched()
{

	Super::StartLaunched();

	// #0. Rotation Offset
	SetActorRelativeRotation(FRotator(0.f, GetActorRotation().Yaw + 180.f, 0.f));

	// #1. Effects
	CheckTrue(MuzzleEffect == nullptr || ProjectileEffect == nullptr);

	if(MuzzleEffect->IsActive() == false)		 MuzzleEffect->Activate();
	if(ProjectileEffect->IsActive() == false)	 ProjectileEffect->Activate();

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

void AProjectile_MagicRay::EndLaunched()
{

	Super::EndLaunched();

	// #0. Effects
	CheckTrue(ProjectileEffect == nullptr || MuzzleEffect == nullptr);

	if (MuzzleEffect->IsActive() == true) MuzzleEffect->DeactivateImmediate();
	if (ProjectileEffect->IsActive() == true) ProjectileEffect->DeactivateImmediate();

}

/*	********************************************************************************************
	
	[Remark	:	BoxComponent�� ���� OnComponentBeginOverlap ���и� ��ü�� BoxTraceMultiForObjects Ȱ��]

	�ð� :	
			22-11-30(��) [00:22]
	���� :	
			Ray ������ Box Tracing ����
	���� :	
			1. Projectile Effect ũ���� Box Trace Multi For Objects ȣ��
			2. Hit Result�� ���� Damage�� ������ Actor���� �����մϴ�.
			3. Projectile_Magic::OnCapsuleBeginOverlap�� Ȱ������ �ʰ�, ������ �������̵� �Լ��� Ȱ���մϴ�.
				* ��?	AMgaic::OffCollision�� ���� Actor�� ���� ������ �浹�� ���ÿ� �߻����� �ʰ�, Ray Projectile�� Ȱ��ȭ �ð��� ������ ���۵ǵ����ϱ� �����Դϴ�.

*	********************************************************************************************/

void AProjectile_MagicRay::BoxTracing()
{
	TArray<TEnumAsByte<EObjectTypeQuery>> DetectedActors;
	TEnumAsByte<EObjectTypeQuery> PawnObjectType = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn);
	DetectedActors.Add(PawnObjectType);

	// Ignored Actorss
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);

	AActor* OwnerWeapon = GetOwner();
	CheckNull(OwnerWeapon);
	IgnoreActors.Add(OwnerWeapon);

	AActor* OwnerActor = OwnerWeapon->GetOwner();
	CheckNull(OwnerActor);
	IgnoreActors.Add(OwnerActor);

	// Hit Result
	TArray<FHitResult> HitResults;

	FVector EndLocation = Sphere->GetForwardVector();
	FVector StartLocation = GetActorLocation();
	StartLocation += GetActorForwardVector() * 800.f;

	bool Result = UKismetSystemLibrary::BoxTraceMultiForObjects(
		GetWorld(),
		StartLocation,
		StartLocation + 10.f,
		FVector(890.f, 100.f, 100.f),
		GetActorRotation(),
		DetectedActors,
		false,
		IgnoreActors,
		EDrawDebugTrace::None,
		HitResults,
		true
	);

	if (Result == true)
	{
		for (const auto& Hit : HitResults)
		{
			OnCapsuleBeginOverlap(
				nullptr,
				Hit.GetActor(),
				nullptr,
				0,
				false,
				Hit
			);
		}
	}

}

void AProjectile_MagicRay::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	Super::OnCapsuleBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

}
 