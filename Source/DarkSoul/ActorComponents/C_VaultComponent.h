#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "C_VaultComponent.generated.h"

DECLARE_DYNAMIC_DELEGATE_FiveParams(FPlayRollAnim, EStateType, InStateType, EWeaponType, InWeaponType, EComboType, InComboType, uint8, InComboCount, uint8, InSkillNum);
DECLARE_DYNAMIC_DELEGATE_FiveParams(FPlayDoubleJumpAnim, EStateType, InStateType, EWeaponType, InWeaponType, EComboType, InComboType, uint8, InComboCount, uint8, InSkillNum);
DECLARE_DYNAMIC_DELEGATE_FiveParams(FPlayParkourAnim, EStateType, InStateType, EWeaponType, InWeaponType, EComboType, InComboType, uint8, InComboCount, uint8, InSkillNum);
DECLARE_DYNAMIC_DELEGATE_FiveParams(FPlayClimbAnim, EStateType, InStateType, EWeaponType ,InWeaponType, EComboType ,InComboType, uint8 ,InComboCount , uint8 ,InSkillNum);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKSOUL_API UC_VaultComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UC_VaultComponent();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
		void StartVault();
	UFUNCTION()
		void EndVault(float InFloat);

	float Vault(class AGameObject** InPlayer);
	float Climb(class AGameObject** InPlayer);

	UFUNCTION()
		void Roll();
	UFUNCTION()
		void DoubleJump();

private:
		bool DetectWall(class AGameObject** InPlayer);

		bool DetectWallFront(class AGameObject** InPlayer);
		bool DetectWallHeight(class AGameObject** InPlayer);
		bool DetectWallDepth(class AGameObject** InPlayer);

		void FixRotation(class AGameObject** InPlayer);
		void FixLocation(class AGameObject** InPlayer, float InFloat1, float InFloat2);

		void OnCollision();
		void OffCollision(AGameObject** InPlayer);

public:
	FPlayRollAnim OnPlayRollAnim;
	FPlayDoubleJumpAnim OnPlayDoubleJumpAnim;
	FPlayParkourAnim OnPlayParkourAnim;
	FPlayClimbAnim OnPlayClimbAnim;


private:
	class AGameObject* OwnerObject;

private:

	bool bCanVault = false;
	bool bCanClimb = false;
	
	FVector FrontWallLocation;
	FVector FrontWallNormalLocation;
	FVector HeightWallLocation;
	FVector DepthWallLocation;

	UPROPERTY(VisibleDefaultsOnly, Category = "Condition")
		float DistanceFrontWall = 120.f;

	// Static Mesh 기준 X축 or Y축 스케일 1.0배 최대	( Vault -> 가능 최대 Depth(X축 or Y축)
	UPROPERTY(VisibleDefaultsOnly, Category = "Condition")
		float DistanceDepthWall = -105.f;

	UPROPERTY(VisibleDefaultsOnly, Category = "Condition")
		float VaultMaxHeight = 60.f;
	// Static Mesh 기준 Z축 스케일 1.7배 최대			( Vault -> 불가능 && Climb -> 가능 )
	UPROPERTY(VisibleDefaultsOnly, Category = "Condition")
		float ClimbMaxHeight = 130.f;

	UPROPERTY(VisibleDefaultsOnly, Category = "Condition")
		float VaultAnimStartLocation = 200.f;
	UPROPERTY(VisibleDefaultsOnly, Category = "Condition")
		float VaultAnimStartHeight = 30.f;
	UPROPERTY(VisibleDefaultsOnly, Category = "Condition")
		float VaultAnimPlayRate = 1.3f;

	UPROPERTY(VisibleDefaultsOnly, Category = "Condition")
		float ClimbAnimStartLocation = 50.f;
	UPROPERTY(VisibleDefaultsOnly, Category = "Condition")
		float ClimbAnimStartHeight = -70.f;
	UPROPERTY(VisibleDefaultsOnly, Category = "Condition")
		float ClimbAnimPlayRate = 1.3f;

};
