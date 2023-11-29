#include "HUD_Profile.h"
#include "Global/Global.h"

#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/EditableTextBox.h"
#include "HUD/HUD_Radar.h"

void UHUD_Profile::NativeOnInitialized()
{

	Super::NativeOnInitialized();

	if (BlankImage)
	{
		WeaponIcon->SetBrushFromTexture(BlankImage);

		Skill01Icon->SetBrushFromTexture(BlankImage);
		SkillIcons.Add(Skill01Icon);

		Skill02Icon->SetBrushFromTexture(BlankImage);
		SkillIcons.Add(Skill02Icon);

		Skill03Icon->SetBrushFromTexture(BlankImage);
		SkillIcons.Add(Skill03Icon);
	}
	else
	{
		PrintLine();
	}


	if (SkillIconBorderImage)
	{
		BorderImage00->SetBrushFromTexture(SkillIconBorderImage);
		BorderImage01->SetBrushFromTexture(SkillIconBorderImage);
		BorderImage02->SetBrushFromTexture(SkillIconBorderImage);
		BorderImage03->SetBrushFromTexture(SkillIconBorderImage);
	}
	else
	{
		PrintLine();
	}

}

void UHUD_Profile::SetHealthBar(float InFloat1, float InFloat2)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(InFloat2 / InFloat1);
	}
	else
	{
		PrintLine();
		return;
	}
}

void UHUD_Profile::SetManaBar(float InFloat1, float InFloat2)
{
	CheckNull(ManaBar);
	ManaBar->SetPercent(InFloat2 / InFloat1);
}

void UHUD_Profile::SetStaminaBar(float InFloat1, float InFloat2)
{
	CheckNull(StaminaBar);
	StaminaBar->SetPercent(InFloat2 / InFloat1);
}

// [23-11-29] : Test
//void UHUD_Profile::SetRadarVisibility()
//{
//	CheckNull(Radar);
//	Radar->SetRadarVisibility();
//}

void UHUD_Profile::SetWeaponIcon(UTexture2D* InTexture)
{
	//FSlateBrush Brush;
	//Brush.SetResourceObject(InTexture);

	if (WeaponIcon)
	{
		WeaponIcon->SetBrushFromTexture(InTexture);
	}
	else
	{
		PrintLine();
		return;
	}

}

void UHUD_Profile::SetSkill01Icon(UTexture2D* InTexture)
{
	if (Skill01Icon)
	{
		Skill01Icon->SetBrushFromTexture(InTexture);
	}
	else
	{
		PrintLine();
		return;
	}
}

void UHUD_Profile::SetSkill02Icon(UTexture2D* InTexture)
{
	CheckNull(Skill02Icon);
	Skill02Icon->SetBrushFromTexture(InTexture);
}

void UHUD_Profile::SetSkill03Icon(UTexture2D* InTexture)
{
	CheckNull(Skill03Icon);
	Skill03Icon->SetBrushFromTexture(InTexture);
}

void UHUD_Profile::SetHSkill01CoolTimeBar(float InFloat1, float InFloat2)
{
	CheckNull(Skill01CoolTimeBar);
	float Percent = FMath::Max(0.f, InFloat2 / InFloat1);
	if (Percent == 0) Percent = 1.f;
	Skill01CoolTimeBar->SetPercent(Percent);
}

void UHUD_Profile::SetHSkill02CoolTimeBar(float InFloat1, float InFloat2)
{
	CheckNull(Skill02CoolTimeBar);
	float Percent = FMath::Max(0.f, InFloat2 / InFloat1);
	if (Percent == 0) Percent = 1.f;
	Skill02CoolTimeBar->SetPercent(Percent);
}

void UHUD_Profile::SetHSkill03CoolTimeBar(float InFloat1, float InFloat2)
{
	CheckNull(Skill03CoolTimeBar);
	float Percent = FMath::Max(0.f, InFloat2 / InFloat1);
	if (Percent == 0) Percent = 1.f;
	Skill03CoolTimeBar->SetPercent(Percent);
}
