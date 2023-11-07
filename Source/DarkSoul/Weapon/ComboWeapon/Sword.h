#pragma once

#include "CoreMinimal.h"
#include "ComboWeapon.h"

#include "Sword.generated.h"

UCLASS()
class DARKSOUL_API ASword : public AComboWeapon
{
	GENERATED_BODY()

public:
	static ASword* Spawn(UWorld* InWorld, class ACharacter* InOwner);

public:
	ASword();

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
	UPROPERTY(VisibleAnywhere)
		class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
		class USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(VisibleAnywhere)
		class UCapsuleComponent* Capsule;


};
