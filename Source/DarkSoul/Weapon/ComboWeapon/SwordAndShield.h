#pragma once

#include "CoreMinimal.h"
#include "Weapon/ComboWeapon/ComboWeapon.h"
#include "SwordAndShield.generated.h"

UCLASS()
class DARKSOUL_API ASwordAndShield : public AComboWeapon
{
	GENERATED_BODY()
	
public:
	ASwordAndShield();

protected:
	virtual void BeginPlay() override;

public:
	virtual void AttachComponentToOwnerComponent(USkeletalMeshComponent* InSkeletalMeshComponent) override;

protected:
	UFUNCTION()
		virtual void OnCollision() override;
	UFUNCTION()
		virtual void OffCollision() override;

private:
	UFUNCTION()
		void OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
		class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		class USkeletalMeshComponent* SkeletalMesh_Sword;
	UPROPERTY(VisibleAnywhere, Category = "Components")
		class UCapsuleComponent* Capsule_Sword;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		class USkeletalMeshComponent* SkeletalMesh_Sheild;
	UPROPERTY(VisibleAnywhere, Category = "Components")
		class UCapsuleComponent* Capsule_Shield;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class USphereComponent* RightFoot;
};
