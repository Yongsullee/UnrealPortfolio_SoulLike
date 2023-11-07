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
	설명:
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
*	[Remark]	: 여느 Magic Projectile과 달리, 관통형 발사체로서 충돌 시 바로 Effect를 Deactivate 하지 않습니다.
* 
*	목적:
*		적 객체와 충돌이 발생해도, Effect가 Deactive 되지 않습니다.
* 
*	설명:
*		1. 충돌 발생을 통해 전달 받은 Other Actor는 nullptr가 아닙니다.
*		2. Expire Handler를 통해 전달 받은 Other Actor는 nullptr 입니다.
*		3. 따라서, Other Actor가 Nullptr일 때만, Projectile Effect를 Deactivate 합니다.
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
	
	[Remark	:	BoxComponent를 통한 OnComponentBeginOverlap 실패를 대체한 BoxTraceMultiForObjects 활용]

	시간 :	
			22-11-30(수) [00:22]
	목적 :	
			Ray 공격의 Box Tracing 구현
	설명 :	
			1. Projectile Effect 크기의 Box Trace Multi For Objects 호출
			2. Hit Result를 통해 Damage를 전달할 Actor들을 선별합니다.
			3. Projectile_Magic::OnCapsuleBeginOverlap을 활용하지 않고, 별도의 오버라이딩 함수를 활용합니다.
				* 왜?	AMgaic::OffCollision을 통해 Actor를 비우는 동작은 충돌과 동시에 발생하지 않고, Ray Projectile의 활성화 시간이 끝나면 동작되도록하기 위함입니다.

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
 