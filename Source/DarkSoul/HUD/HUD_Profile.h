#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "HUD_Profile.generated.h"

UCLASS()
class DARKSOUL_API UHUD_Profile : public UUserWidget
{
	GENERATED_BODY()


// ************************************************************************************
		// Method
// ************************************************************************************

protected:
	virtual void NativeOnInitialized();

// Progress Bar
public:
	// #1. HP Bar
	void SetHealthBar(float InFloat1, float InFloat2);
	// #2. MP Bar
	void SetManaBar(float InFloat1, float InFloat2);
	// #3. SP Bar
	void SetStaminaBar(float InFloat1, float InFloat2);

// Imgaes
public:
	// Weapon
	void SetWeaponIcon(UTexture2D* InTexture);

public:
	// Skill
	void SetSkill01Icon(UTexture2D* InTexture);
	void SetSkill02Icon(UTexture2D* InTexture);
	void SetSkill03Icon(UTexture2D* InTexture);

public:
	// CoolTimes
	void SetHSkill01CoolTimeBar(float InFloat1, float InFloat2);
	void SetHSkill02CoolTimeBar(float InFloat1, float InFloat2);
	void SetHSkill03CoolTimeBar(float InFloat1, float InFloat2);

//public:
//	// [23-11-26] : On Prototyping
//	void SetRadarVisibility();
//	FORCEINLINE class UHUD_Radar* GetRadar() { return Radar; }

// ************************************************************************************
		// Data Member
// ************************************************************************************


// Progress Bar
	// #1. HP, MP, SP
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* HealthBar;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* ManaBar;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* StaminaBar;
	// #2. Skills
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* Skill01CoolTimeBar;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* Skill02CoolTimeBar;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* Skill03CoolTimeBar;


// Image
	// #1. Profile
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* ProfileImage;
	// #2. Weapon
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* BorderImage00;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* WeaponIcon;
	// #3. Skills
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* Skill01Icon;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* BorderImage01;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* Skill02Icon;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* BorderImage02;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* Skill03Icon;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* BorderImage03;

private:
	TArray<UImage*> SkillIcons;

// Texture
protected:
	UPROPERTY(EditDefaultsOnly)
		UTexture2D* BlankImage;
	UPROPERTY(EditDefaultsOnly)
		UTexture2D* SkillIconBorderImage;


protected:
	// [23-11-26] : On Proto
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UHUD_Radar* Radar;


};
