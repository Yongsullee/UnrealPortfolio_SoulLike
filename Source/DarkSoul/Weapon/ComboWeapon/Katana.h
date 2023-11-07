#pragma once

#include "CoreMinimal.h"
#include "Weapon/ComboWeapon/ComboWeapon.h"

#include "Katana.generated.h"

UCLASS()
class DARKSOUL_API AKatana : public AComboWeapon
{
	GENERATED_BODY()

public:
	AKatana();

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
	UFUNCTION()
		void OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
		class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		class USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		class UCapsuleComponent* Capsule;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class USphereComponent* RightFoot;
};
