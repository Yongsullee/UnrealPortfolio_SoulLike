#include "Projectile.h"
#include "Global/Global.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Weapon/Weapon.h"
#include "GameFramework/Character.h"

AProjectile::AProjectile()
{

	PrimaryActorTick.bCanEverTick = false;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));

	CheckNull(ProjectileMovement);
	ProjectileMovement->Velocity = FVector::ZeroVector;
	ProjectileMovement->bInitialVelocityInLocalSpace = true;
	ProjectileMovement->InitialSpeed = 5000.f;
	ProjectileMovement->MaxSpeed = 10000.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
	InitialLifeSpan = 0.f;

}

void AProjectile::BeginPlay()
{

	Super::BeginPlay();

	// #1 Velocity
	CheckTrue(ProjectileMovement == nullptr);
	ProjectileMovement->SetVelocityInLocalSpace(FVector(0.f, 0.f, 0.f));
	
}

void AProjectile::StartLaunched()
{

	// #1. Expire Handler
	GetWorld()->GetTimerManager().SetTimer(ExpireHandler, FTimerDelegate::CreateLambda([&]() {
		EndLaunched();
		}), ExpireRate, false);

}


/*	************************************************************************************************************
* 
*	[Remark_23-03-20]
* 
*	목적:	생명 주기를 관리합니다.
*	
*	설명:
*		1.	OtherActor가 nullptr인 경우, Expire Rate에 의한 호출이므로, 정상적으로 End Launched가 호출됩니다.
*		2.	OtherActor가 nullptr가 아닌 경우, 충돌에 의한 호출이므로, bPenetrate 변수를 통해 관통 발사체 여부를 체크합니다.
*		3.	 관통형 발사체(bPenetrate)일 경우, 충돌이 발생해도 지속적으로 이동합니다.
*		4.	관통형 발사체(bPenetrate)가 아닐 경우, 충돌이 발생하면 멈춥니다.
* 
*	************************************************************************************************************/

void AProjectile::EndLaunched()
{

	// #0. Projectile Stop & Timer Stops
	 ProjectileMovement->SetVelocityInLocalSpace(FVector::ZeroVector);
	GetWorld()->GetTimerManager().ClearTimer(ExpireHandler);

	// #1. Delegate
	OnProjectileEndLaunched.ExecuteIfBound(ProjectileType, PoolObjectInfo_Struct.Index);

	// #2. Attack Hit Actors 지워주기
	AWeapon* OwnerWeapon = Cast<AWeapon>(GetOwner());
	CheckTrue(OwnerWeapon == nullptr);
	OwnerWeapon->OffCollision();

	GetWorld()->GetTimerManager().SetTimer(OffVisibilityHandler, FTimerDelegate::CreateLambda([&]() {
		SetActorHiddenInGame(true);
		}), OffVisibilityRate, false);

}

void AProjectile::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	OnProjectileBeginOverlap.ExecuteIfBound(OtherActor, ProjectileType, PoolObjectInfo_Struct.Index);

}
