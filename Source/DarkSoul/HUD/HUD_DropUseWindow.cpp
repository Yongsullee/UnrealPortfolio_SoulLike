#include "HUD_DropUseWindow.h"
#include "Global/Global.h"

#include "Components/Button.h"


void UHUD_DropUseWindow::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UseButton->OnClicked.AddDynamic(this, &UHUD_DropUseWindow::OnUseButton);
	DropButton->OnClicked.AddDynamic(this, &UHUD_DropUseWindow::OnDropButton);

}

void UHUD_DropUseWindow::OnUseButton()
{

	OnUseButtonClicked.Broadcast();

}

void UHUD_DropUseWindow::OnDropButton()
{

	OnDropButtonClicked.Broadcast();

}
