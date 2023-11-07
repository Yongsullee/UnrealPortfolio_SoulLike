// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ComboWeapon.h"
#include "Axe.generated.h"

/**
 *
 */
UCLASS()
class DARKSOUL_API AAxe : public AComboWeapon
{
	GENERATED_BODY()

public:
	static AAxe* Spawn(UWorld* InWorld, class ACharacter* InOwner);

public:
	AAxe();

protected:
	virtual void BeginPlay() override;

public:
	virtual void AttachComponentToOwnerComponent(USkeletalMeshComponent* InSkeletalMeshComponent) override;


protected:
	UFUNCTION()
		virtual void OnCollision() override;
	UFUNCTION()
		virtual void OffCollision() override;

	UFUNCTION()
		void OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
private:
	UPROPERTY(VisibleAnywhere)
		class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
		class USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(VisibleAnywhere)
		class UCapsuleComponent* Capsule;
};
