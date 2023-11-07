#include "HUD_EnemyProfile.h"
#include "Global/Global.h"

#include "Components/ProgressBar.h"

void UHUD_EnemyProfile::NativeConstruct()
{
	Super::NativeConstruct();

}


void UHUD_EnemyProfile::SetEnemyHealthBar(float InFloat1, float InFloat2)
{

	CheckNull(EnemyHealthBar);
	EnemyHealthBar->SetPercent(InFloat2/InFloat1);

}