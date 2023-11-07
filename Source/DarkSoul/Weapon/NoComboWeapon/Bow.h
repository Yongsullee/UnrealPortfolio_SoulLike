#pragma once

#include "CoreMinimal.h"
#include "Weapon/NoComboWeapon/NoComboWeapon.h"

#include "Bow.generated.h"

UCLASS()
class DARKSOUL_API ABow : public ANoComboWeapon
{
	GENERATED_BODY()

public:
	ABow();

protected:
	virtual void BeginPlay() override;

private:
	virtual void Tick(float DeltaTime) override;

public:
	virtual void AttachComponentToOwnerComponent(USkeletalMeshComponent* InSkeletalMeshComponent) override;

public:
	virtual void Attack() override;
	virtual void EndAttack() override;

public:
	void PullString();
	void ReleaseString();

private:
	UFUNCTION()
		void ArrowBeginOverlap(AActor* OtherActor);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class USceneComponent* Root;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class UCapsuleComponent* Capsule;

private:
	const FName BowSocket = TEXT("Bow_Projectile");

	const float Max_StringAlpha = 1.f;

	UPROPERTY(VisibleAnywhere, Category = "Anim Instance")
		float StringAlpha = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Bow")
		float ReleaseRate = 1.f;

	bool bStringReleased = false;

public:
	FORCEINLINE class USkeletalMeshComponent* GetSkeletalMesh() { return SkeletalMesh; }
	FORCEINLINE float GetStringAlpha() { return StringAlpha; }
	FORCEINLINE bool IsStringReleased() { return bStringReleased; }
	FORCEINLINE const FName GetBowSocketName() const { return BowSocket; }

};
