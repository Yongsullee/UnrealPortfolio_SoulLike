#include "C_HUDComponent.h"
#include "Global/Global.h"

#include "HUD/HUD_Profile.h"

#include "GameObjects/PlayerCharacter.h"

#include "ActorComponents/C_HealthComponent.h"
#include "ActorComponents/C_ManaComponent.h"
#include "ActorComponents/C_WeaponComponent.h"
#include "ActorComponents/C_StaminaComponent.h"

UC_HUDComponent::UC_HUDComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UC_HUDComponent::BeginPlay()
{

	Super::BeginPlay();
	
	AddProfileHUD();

	AActor* OwnerActor = GetOwner();
	APlayerCharacter* OwnerPlayer = Cast<APlayerCharacter>(OwnerActor);

	UC_HealthComponent* HealthComp = OwnerActor->FindComponentByClass<UC_HealthComponent>();
	UC_ManaComponent* ManaComp = OwnerActor->FindComponentByClass<UC_ManaComponent>();
	UC_StaminaComponent* StaminaComp = OwnerActor->FindComponentByClass<UC_StaminaComponent>();
	UC_WeaponComponent* WeaponComp = OwnerActor->FindComponentByClass<UC_WeaponComponent>();

	CheckTrue(!OwnerActor || !OwnerPlayer || !HealthComp || !ManaComp || !StaminaComp || !WeaponComp);
	
	HealthComp->OnUpdateHUDProfileHealthBar.BindUFunction(this, "UpdateHUDProfileHealthBar");
	ManaComp->OnUpdateHUDProfileManaBar.BindUFunction(this, "UpdateHUDProfileManaBar");
	ManaComp->OnUpdateHUDProfileSkillCoolTime.AddDynamic(this, &UC_HUDComponent::UpdateHUDProfileSkillCoolTime);
	StaminaComp->OnUpdateHUDProfileStaminaBar.BindUFunction(this, "UpdateHUDProfileStaminaBar");
	WeaponComp->OnUpdateHUDWeaponImage.BindUFunction(this, "UpdateHUDProfileWeaponImage");
	WeaponComp->OnUpdateHUDSkillIcons.BindUFunction(this, "UpdateHUDProfileSkillIcons");
	OwnerPlayer->OnShowCompassWidget.BindUFunction(this, "UpdateCompassVisibility");

}


void UC_HUDComponent::AddProfileHUD()
{
	CheckNull(HUDProfileClass);

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	CheckNull(PlayerController);

	HUDProfile = CreateWidget<UHUD_Profile>(PlayerController, HUDProfileClass);

	CheckNull(HUDProfile);
	HUDProfile->AddToViewport();
}

void UC_HUDComponent::UpdateHUDProfileHealthBar(float InFloat1, float InFloat2)
{
	CheckNull(HUDProfile);
	HUDProfile->SetHealthBar(InFloat1, InFloat2);
}

void UC_HUDComponent::UpdateHUDProfileManaBar(float InFloat1, float InFloat2)
{
	CheckNull(HUDProfile);
	HUDProfile->SetManaBar(InFloat1, InFloat2);
}

void UC_HUDComponent::UpdateHUDProfileStaminaBar(float InFloat1, float InFloat2)
{
	CheckNull(HUDProfile);
	HUDProfile->SetStaminaBar(InFloat1, InFloat2);
}

void UC_HUDComponent::UpdateCompassVisibility()
{
	CheckNull(HUDProfile);
	HUDProfile->SetRadarVisibility();
}


void UC_HUDComponent::UpdateHUDProfileWeaponImage(UTexture2D* InTexture)
{
	CheckNull(HUDProfile);
	HUDProfile->SetWeaponIcon(InTexture);
}

void UC_HUDComponent::UpdateHUDProfileSkillIcons(UTexture2D* InSkill01Icon, UTexture2D* InSkill02Icon, UTexture2D* InSkill03Icon)
{
	CheckNull(HUDProfile);
	HUDProfile->SetSkill01Icon(InSkill01Icon);
	HUDProfile->SetSkill02Icon(InSkill02Icon);
	HUDProfile->SetSkill03Icon(InSkill03Icon);
}

void UC_HUDComponent::UpdateHUDProfileSkillCoolTime(int32 InSkillNum, float InFloat1, float InFloat2)
{
	CheckNull(HUDProfile);
	switch(InSkillNum)
	{
	case 0:
		HUDProfile->SetHSkill01CoolTimeBar(InFloat1, InFloat2);
		break;
	case 1:
		HUDProfile->SetHSkill02CoolTimeBar(InFloat1, InFloat2);
		break;
	case 2:
		HUDProfile->SetHSkill03CoolTimeBar(InFloat1, InFloat2);
		break;
	}
}

