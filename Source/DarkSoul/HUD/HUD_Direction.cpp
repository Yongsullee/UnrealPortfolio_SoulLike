#include "HUD_Direction.h"
#include "Global/Global.h"

#include "Components/EditableTextBox.h"

void UHUD_Direction::NativePreConstruct()
{

	DirectionText->SetText(DirectionInfoStruct.Name);

	Super::NativePreConstruct();

}