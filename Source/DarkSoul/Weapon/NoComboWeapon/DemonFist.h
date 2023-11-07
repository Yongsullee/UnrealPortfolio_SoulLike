#pragma once

#include "CoreMinimal.h"
#include "Weapon/NoComboWeapon/NoComboWeapon.h"

#include "DemonFist.generated.h"

UCLASS()
class DARKSOUL_API ADemonFist : public ANoComboWeapon
{
	GENERATED_BODY()
	
public:
	ADemonFist();

protected:
	virtual void BeginPlay() override;

private:
	virtual void Tick(float DeltaTime) override;

public:
	virtual void AttachComponentToOwnerComponent(USkeletalMeshComponent* InSkeletalMeshComponent) override;

protected:
	UFUNCTION()
		virtual void OnCollision() override;
	UFUNCTION()
		virtual void OffCollision() override;

protected:
	virtual void AttackSkill(const int InSkillNum) override;
	virtual void EndAttackSkill() override;

private:
	void CreateMagneticField();
	void Dash();

private:
	UFUNCTION()
		void OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void MagicProjectileBeginOverlap(AActor* OtherActor);

private:
	class USceneComponent* Root;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class USphereComponent* LeftHand;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class USphereComponent* RightHand;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class USphereComponent* Wing;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class USphereComponent* Mouth;

private:
	TArray<USphereComponent*> SphereComponents;
	static const uint8 SphereComponentsSize = 5;

private:
	bool bOnMagneticField = false;
	const float EnemyDetectionMaxRadius = 500.f;
	const float DashDistance = 1000.f;

};
