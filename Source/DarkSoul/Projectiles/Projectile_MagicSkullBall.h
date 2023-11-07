#pragma once

#include "CoreMinimal.h"
#include "Projectiles/Projectile_Magic.h"

#include "Projectile_MagicSkullBall.generated.h"

UCLASS()
class DARKSOUL_API AProjectile_MagicSkullBall : public AProjectile_Magic
{
	GENERATED_BODY()

public:
	AProjectile_MagicSkullBall();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

//	******************************************************************************************
	//	Method
//	******************************************************************************************

public:
	virtual void OnCollision() override;
	virtual void OffCollision() override;

protected:
	virtual void StartLaunched() override;
	virtual void EndLaunched() override;

private:
	virtual void OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;


// *******************************************************************************************
	// Data Member
// *******************************************************************************************

private:

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class USphereComponent* Sphere;

};
