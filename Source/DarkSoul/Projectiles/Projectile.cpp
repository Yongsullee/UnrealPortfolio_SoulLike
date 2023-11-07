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
*	����:	���� �ֱ⸦ �����մϴ�.
*	
*	����:
*		1.	OtherActor�� nullptr�� ���, Expire Rate�� ���� ȣ���̹Ƿ�, ���������� End Launched�� ȣ��˴ϴ�.
*		2.	OtherActor�� nullptr�� �ƴ� ���, �浹�� ���� ȣ���̹Ƿ�, bPenetrate ������ ���� ���� �߻�ü ���θ� üũ�մϴ�.
*		3.	 ������ �߻�ü(bPenetrate)�� ���, �浹�� �߻��ص� ���������� �̵��մϴ�.
*		4.	������ �߻�ü(bPenetrate)�� �ƴ� ���, �浹�� �߻��ϸ� ����ϴ�.
* 
*	************************************************************************************************************/

void AProjectile::EndLaunched()
{

	// #0. Projectile Stop & Timer Stops
	 ProjectileMovement->SetVelocityInLocalSpace(FVector::ZeroVector);
	GetWorld()->GetTimerManager().ClearTimer(ExpireHandler);

	// #1. Delegate
	OnProjectileEndLaunched.ExecuteIfBound(ProjectileType, PoolObjectInfo_Struct.Index);

	// #2. Attack Hit Actors �����ֱ�
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
