#pragma once

#include "CoreMinimal.h"
#include "Projectiles/Projectile.h"

#include "Projectile_Arrow.generated.h"

UCLASS()
class DARKSOUL_API AProjectile_Arrow : public AProjectile
{
	GENERATED_BODY()

public:
	AProjectile_Arrow();

protected:
	virtual void BeginPlay() override;

private:
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void OnCollision() override;
	virtual void OffCollision() override;

public:
	virtual void StartLaunched() override;
	virtual void EndLaunched() override;

private:
	void OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class USceneComponent* Root;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class UArrowComponent* Arrow;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		class USphereComponent* Sphere;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class UNiagaraComponent* ImpactEffect;

};
