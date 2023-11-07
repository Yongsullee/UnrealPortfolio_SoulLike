#include "HUD_Radar.h"
#include "Global/Global.h"

#include "HUD/HUD_Direction.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"

#include "GameObjects/PlayerCharacter.h"
#include "Camera/CameraComponent.h"


// [23-07-08] Radar
void UHUD_Radar::NativeOnInitialized()
{
	for (uint8 i = 0; i < 4; i++)
	{
		CreateDirectionWidgets(DirectionInfos[i]);
	}

	FillAnimationsMap(); 

	Super::NativeOnInitialized();

}

void UHUD_Radar::FillAnimationsMap()
{
	AnimationsMap.Empty();

	UProperty* Prop = GetClass()->PropertyLink;

	while (Prop != nullptr)
	{
		if (Prop->GetClass() == UObjectProperty::StaticClass())
		{
			UObjectProperty* ObjProp = Cast<UObjectProperty>(Prop);

			if (ObjProp->PropertyClass == UWidgetAnimation::StaticClass())
			{
				UObject* Obj = ObjProp->GetObjectPropertyValue_InContainer(this);

				UWidgetAnimation* WidgetAnim = Cast<UWidgetAnimation>(Obj);

				if (WidgetAnim != nullptr && WidgetAnim->MovieScene != nullptr)
				{
					FName AnimName = WidgetAnim->MovieScene->GetFName();
					AnimationsMap.Add(AnimName, WidgetAnim);
				}
			}
		}

		Prop = Prop->PropertyLinkNext;
	}
}

UWidgetAnimation* UHUD_Radar::GetAnimationByName(FName AnimationName) const
{
	UWidgetAnimation* const* WidgetAnim = AnimationsMap.Find(AnimationName);
	if (WidgetAnim)
	{
		return *WidgetAnim;
	}
	return nullptr;
}

bool UHUD_Radar::PlayAnimationByName(FName AnimationName, float StartAtTime, int32 NumLoopsToPlay, EUMGSequencePlayMode::Type PlayMode, float PlaybackSpeed)
{
	UWidgetAnimation* WidgetAnim = GetAnimationByName(AnimationName);
	if (WidgetAnim)
	{
		PlayAnimation(WidgetAnim, StartAtTime, NumLoopsToPlay, PlayMode, PlaybackSpeed);
		return true;
	}
	return false;
}


void UHUD_Radar::SetRadarVisibility()
{
	if (bVisible == true)
	{
		StartBlendOut();
	}
	else
	{
		StartBlendIn();
	}
}

void UHUD_Radar::StartBlendOut()
{

	bool Result = PlayAnimationByName("BlendOut", 0.f, 1, EUMGSequencePlayMode::Forward, 1.f);
	if (Result) {
		FTimerHandle BlendOutTimer;
		GetWorld()->GetTimerManager().SetTimer(BlendOutTimer, FTimerDelegate::CreateLambda([&]() {
			SetVisibility(ESlateVisibility::Hidden);	
		
			bVisible = false;
			}), 1.f, false);
	}

}

void UHUD_Radar::StartBlendIn()
{

	if (bVisible == false)
	{
		SetVisibility(ESlateVisibility::Visible);
		PlayAnimationByName("BlendOut", 0.f, 1, EUMGSequencePlayMode::Reverse, 1.f);
		
		bVisible = true;
	}

}

void UHUD_Radar::CreateDirectionWidgets(FDirectionInfo InStruct)
{

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	CheckNull(PlayerController);

	UHUD_Direction* TmpWidget = CreateWidget<UHUD_Direction>(PlayerController, DirectionWidgetClass);
	TmpWidget->SetDirectionInfoStruct(InStruct);
	TmpWidget->AddToViewport();
	TmpWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	UOverlaySlot* OverlaySlot = Overlay->AddChildToOverlay(TmpWidget);

	OverlaySlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
	OverlaySlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Bottom);

	DirectionWidgets.Add(TmpWidget);

}