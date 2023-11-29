#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "C_HUDComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKSOUL_API UC_HUDComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UC_HUDComponent();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
		void AddProfileHUD();
private:
	UFUNCTION()
		void UpdateHUDProfileHealthBar(float InFloat1, float InFloat2);
	UFUNCTION()
		void UpdateHUDProfileManaBar(float InFloat1, float InFloat2);
	UFUNCTION()
		void UpdateHUDProfileStaminaBar(float InFloat1, float InFloat2);

//public:
//	UFUNCTION()
//		void UpdateCompassVisibility();

private:
	UFUNCTION()
		void UpdateHUDProfileWeaponImage(class UTexture2D* InTexture);

private:
	UFUNCTION()
		void UpdateHUDProfileSkillIcons(class UTexture2D* InSkill01Icon, class UTexture2D* InSkill02Icon, class UTexture2D* InSkill03Icon);
	UFUNCTION()
		void UpdateHUDProfileSkillCoolTime(int32 SkillNum, float InFloat1, float InFloat2);


private:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<class UHUD_Profile> HUDProfileClass;

	UPROPERTY(VisibleAnywhere, Category = "UI")
		class UHUD_Profile* HUDProfile;

private:
	const float MaxDirectionWidgetTranslation = 330.f;

};
