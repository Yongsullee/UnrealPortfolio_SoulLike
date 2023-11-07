#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "HUD_Profile.generated.h"

UCLASS()
class DARKSOUL_API UHUD_Profile : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized();

public:
	void SetHealthBar(float InFloat1, float InFloat2);
	void SetManaBar(float InFloat1, float InFloat2);
	void SetStaminaBar(float InFloat1, float InFloat2);

public:
	void SetRadarVisibility();
	FORCEINLINE class UHUD_Radar* GetRadar() { return Radar; }

public:
	void SetWeaponIcon(UTexture2D* InTexture);

public:
	void SetSkill01Icon(UTexture2D* InTexture);
	void SetSkill02Icon(UTexture2D* InTexture);
	void SetSkill03Icon(UTexture2D* InTexture);

public:
	void SetHSkill01CoolTimeBar(float InFloat1, float InFloat2);
	void SetHSkill02CoolTimeBar(float InFloat1, float InFloat2);
	void SetHSkill03CoolTimeBar(float InFloat1, float InFloat2);

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* ProfileImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* HealthBar;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* ManaBar;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* StaminaBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UHUD_Radar* Radar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* BorderImage00;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UImage* WeaponIcon;

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

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* Skill01CoolTimeBar;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* Skill02CoolTimeBar;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* Skill03CoolTimeBar;

protected:
	UPROPERTY(EditDefaultsOnly)
		UTexture2D* BlankImage;
	UPROPERTY(EditDefaultsOnly)
		UTexture2D* SkillIconBorderImage;

private:
	TArray<UImage*> SkillIcons;

};
