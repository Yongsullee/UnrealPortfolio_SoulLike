#pragma once

#include "CoreMinimal.h"
#include "Weapon/ComboWeapon/ComboWeapon.h"
#include "Fist.generated.h"

UCLASS()
class DARKSOUL_API AFist : public AComboWeapon
{
	GENERATED_BODY()

public:
	AFist();

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
	class USceneComponent* Root;

	TArray<class USphereComponent*> SphereComponents;
	static const uint8 SphereComponentsSize = 4;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class USphereComponent* LeftHand;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class USphereComponent* RightHand;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class USphereComponent* LeftFoot;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class USphereComponent* RightFoot;

};
