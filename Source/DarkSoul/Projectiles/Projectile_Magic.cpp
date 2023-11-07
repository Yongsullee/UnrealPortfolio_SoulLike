#include "Projectile_Magic.h"
#include "Global/Global.h"

#include "NiagaraComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameObjects/GameObject.h"

#include"Weapon/Weapon.h"

AProjectile_Magic::AProjectile_Magic()
{

	PrimaryActorTick.bCanEverTick = false;

}

void AProjectile_Magic::BeginPlay()
{

	Super::BeginPlay();

}

void AProjectile_Magic::OnCollision()
{

	Super::OnCollision();

}

void AProjectile_Magic::OffCollision()
{

	Super::OffCollision();

}


void AProjectile_Magic::StartLaunched()
{

	Super::StartLaunched();

	// #0. Projectile Movement
	CheckTrue(ProjectileMovement == nullptr);
	ProjectileMovement->SetVelocityInLocalSpace(FVector(-1.f, 0.f, 0.f) * ProjectileMovement->InitialSpeed);

	// #1. Muzzle Timer On
	if (!!MuzzleEffect && OffMuzzleEffectRate >= 0.f)
	{
		GetWorld()->GetTimerManager().SetTimer(OffMuzzleEffectHandler, FTimerDelegate::CreateLambda([&]() {
			MuzzleEffect->DeactivateImmediate();
			}), OffMuzzleEffectRate, false);
	}

}


void AProjectile_Magic::EndLaunched()
{
	
	Super::EndLaunched();

	// #1. Hit Timer On
	if (!!HitEffect && OffHitEffectRate >= 0.f)
	{
		GetWorld()->GetTimerManager().SetTimer(OffHitEffectHandler, FTimerDelegate::CreateLambda([&]() {
			HitEffect->DeactivateImmediate();
			}), OffHitEffectRate, false);
	}

}


void AProjectile_Magic::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	

	CheckTrue(!OtherActor);

	AActor* OwnerWeapon = GetOwner();
	AGameObject* OwnerObject= Cast<AGameObject>(OwnerWeapon->GetOwner());
	AGameObject* OtherObject = Cast<AGameObject>(OtherActor);
	
	CheckTrue(!OtherObject || !OwnerWeapon || !OwnerObject 
		|| OtherActor == OwnerWeapon || OtherActor == OwnerObject || OwnerObject->IsHostile(OtherObject) == false);

	Super::OnCapsuleBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if(bPenetrable == false) EndLaunched();

}
