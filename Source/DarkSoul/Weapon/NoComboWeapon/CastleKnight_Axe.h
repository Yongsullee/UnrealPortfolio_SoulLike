#pragma once

#include "CoreMinimal.h"
#include "NoComboWeapon.h"
#include "CastleKnight_Axe.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOUL_API ACastleKnight_Axe : public ANoComboWeapon
{
	GENERATED_BODY()


public:
	static ACastleKnight_Axe* Spawn(UWorld* InWorld, class ACharacter* InOwner);

public:
	ACastleKnight_Axe();

protected:
	virtual void BeginPlay() override;

public:
	virtual void AttachComponentToOwnerComponent(USkeletalMeshComponent* InSkeletalMeshComponent) override;

protected:

	UFUNCTION()
		virtual void OnCollision() override;
	UFUNCTION()
		virtual void OffCollision() override;

public:
	UFUNCTION(Category="Collision")
		void OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:

	// 0. Components			******************************************

	UPROPERTY(VisibleAnywhere)
		class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere)
		class UCapsuleComponent* Capsule;
};
