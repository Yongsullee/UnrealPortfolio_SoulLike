#pragma once

#include "CoreMinimal.h"
#include "GameObject.h"
#include "Global/Global.h"
#include "Interface/OwnerWeaponInterface.h"

#include "PlayerCharacter.generated.h"

// #0. Inventory Comp
DECLARE_DYNAMIC_DELEGATE_OneParam(FShowInventoryWidget, EItemType, InItemType);
DECLARE_DYNAMIC_DELEGATE(FRemoveInventoryWidget);

DECLARE_DYNAMIC_DELEGATE(FAcquireItem);

// #1. HUD Comp
DECLARE_DYNAMIC_DELEGATE(FShowCompassWidget);

// #2. Portal
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSwitchLevel, AActor*, InActor);

UCLASS()
class DARKSOUL_API APlayerCharacter : public AGameObject
{
	GENERATED_BODY()

public:
	APlayerCharacter();

protected:
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

// ************************************************************************************
	// Method
// ************************************************************************************

private:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

private:
	virtual void ChangeColor() override;
	virtual void RestoreColor() override;

public:
	void EquipSword();
	void EquipAxe();
	void EquipFist();
	void EquipKatana();
	void EquipSwordAndShield();
	void EquipWizard();

	void ChangePlayerComboType();

private:
	void StartRoll();

public:
	UFUNCTION(BlueprintCallable)
		void EndRoll();

private:
	UFUNCTION()
		void LockOnTarget();
	UFUNCTION()
		void CycleClockwise();
	UFUNCTION()
		void CycleAntiClockwise();

private:
	void OpenInventory();
	void AcquireItem();

private:
	void OnCompass();

private:
	// [23-07-07] Level Change
	void OnChangeLevel();

public:
	FShowInventoryWidget OnShowInventoryWidget;
	FRemoveInventoryWidget OnRemoveInventoryWidget;
	FAcquireItem OnAcquireItem;

public:
	FShowCompassWidget OnShowCompassWidget;

public:
	FSwitchLevel OnSwitchLevel;

// *****************************************************************************

	// Data Member

// *****************************************************************************

private:
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
		class USpringArmComponent* SpringArm;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
		class UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
		TSubclassOf<class UMatineeCameraShake> CameraShakeMinor;

private:

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class UC_ClassComponent* ClassComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class UC_TargetComponent* TargetComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class UC_HUDComponent* HUDComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class UC_InventoryComponent* InventoryComponent;

// [23-07-21] : Change Color
private:
	class UMaterialInstanceDynamic* OutfitMaterial;
	class UMaterialInstanceDynamic* HeadMaterial;
	class UMaterialInstanceDynamic* BodyMaterial;
	class UMaterialInstanceDynamic* HairMaterial;

private:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UHUD_Profile> HUDProfileClass;
	UPROPERTY(VisibleAnywhere, Category = "UI")
		class UHUD_Profile* HUDProfile;


// ************************************************************************************
	// Getter & Setter
// ************************************************************************************

public:
	FORCEINLINE UCameraComponent* GetCameraComponent() { return Camera; }

	FORCEINLINE UC_StateComponent* GetStateComponent() { return StateComponent; }
	FORCEINLINE UC_StaminaComponent* GetStaminaComponent() { return StaminaComponent; }
	FORCEINLINE UC_InventoryComponent* GetInventoryComponent() { return InventoryComponent; }

};
