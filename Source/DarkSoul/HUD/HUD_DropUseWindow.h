#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "HUD_DropUseWindow.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUseButtonClicked);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDropButtonClicked);

UCLASS()
class DARKSOUL_API UHUD_DropUseWindow : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized();

private:
	UFUNCTION()
		void OnUseButton();
	UFUNCTION()
		void OnDropButton();

public:
	FUseButtonClicked OnUseButtonClicked;
	FDropButtonClicked OnDropButtonClicked;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* UseButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UEditableTextBox* UseText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UButton* DropButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UEditableTextBox* DropText;

public:
	FORCEINLINE UButton* GetUseButton() { return UseButton; }
	FORCEINLINE UButton* GetDropButton() { return DropButton; }


};
