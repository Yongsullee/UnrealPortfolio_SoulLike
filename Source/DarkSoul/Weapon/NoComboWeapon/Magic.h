#pragma once

#include "CoreMinimal.h"
#include "Weapon/NoComboWeapon/NoComboWeapon.h"
#include "Global/Global.h"

#include "Magic.generated.h"

UCLASS()
class DARKSOUL_API AMagic : public ANoComboWeapon
{
	GENERATED_BODY()

public:
	AMagic();

protected:
	virtual void BeginPlay() override;

//	*****************************************************************************
	// Method
//	*****************************************************************************

public:
	virtual void AttachComponentToOwnerComponent(USkeletalMeshComponent* InSkeletalMeshComponent) override;


public:
	virtual void Attack() override;
	virtual void EndAttack() override;

	virtual void AttackSkill(const int InSkillNum) override;
	virtual void EndAttackSkill() override;

private:
	UFUNCTION()
		void MagicProjectileBeginOverlap(AActor* OtherActor);

// *****************************************************************************
	// Data Member
// *****************************************************************************

private:
	class USceneComponent* Root;

	TArray<class USphereComponent*> SphereComponents;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class USphereComponent* LeftHand;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class USphereComponent* RightHand;

};
