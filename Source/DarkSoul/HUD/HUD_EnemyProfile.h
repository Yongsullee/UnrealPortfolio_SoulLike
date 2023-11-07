#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "HUD_EnemyProfile.generated.h"

UCLASS()
class DARKSOUL_API UHUD_EnemyProfile : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	void SetEnemyHealthBar(float InFloat1, float InFloat2);
	void SetStatusIcon(UTexture2D* InTexture);


protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UProgressBar* EnemyHealthBar;

};
