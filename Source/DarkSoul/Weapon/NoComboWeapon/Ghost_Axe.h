#pragma once

#include "CoreMinimal.h"
#include "Weapon/NoComboWeapon/NoComboWeapon.h"

#include "Ghost_Axe.generated.h"

UCLASS()
class DARKSOUL_API AGhost_Axe : public ANoComboWeapon
{
	GENERATED_BODY()
	
public:
	AGhost_Axe();

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
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class USceneComponent* Root;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class USphereComponent* Axe;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class USphereComponent* LeftHand_Cast_01;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class USphereComponent* LeftHand_Cast_02;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class USphereComponent* RightHand_Cast_01;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class USphereComponent* RightHand_Cast_02;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class UParticleSystemComponent* RightHand_Cast_01_SkillEffect;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class UParticleSystemComponent* LeftHand_Cast_02_SkillEffect;


private:
	TArray<USphereComponent*> SphereComponents;
	static const uint8 SphereComponentsSize = 2;

private:
	bool bOnMagneticField = false;
	const float EnemyDetectionMaxRadius = 500.f;
	const float DashDistance = 1200.f;

};
