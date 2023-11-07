#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.h"

#include "Projectile_Magic.generated.h"


UCLASS()
class DARKSOUL_API AProjectile_Magic : public AProjectile
{
	GENERATED_BODY()
	
public:	
	AProjectile_Magic();

protected:
	virtual void BeginPlay() override;

//	******************************************************************************************
	//	Method
//	******************************************************************************************

protected:
	virtual void OnCollision();
	virtual void OffCollision();

protected:
	virtual void StartLaunched() override;
	virtual void EndLaunched() override;

protected:
	virtual void OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;


// *******************************************************************************************
	// Data Member
// *******************************************************************************************

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class USceneComponent* Scene;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class UNiagaraComponent* ProjectileEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class UNiagaraComponent* MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class UNiagaraComponent* HitEffect;

protected:
	FTimerHandle OffMuzzleEffectHandler;
	float OffMuzzleEffectRate = -1.f;

	FTimerHandle OffHitEffectHandler;
	float OffHitEffectRate = -1.f;

};
