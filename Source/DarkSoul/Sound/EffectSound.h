#pragma once

#include "CoreMinimal.h"
#include "Sound/Sound.h"

#include "EffectSound.generated.h"

UCLASS()
class DARKSOUL_API AEffectSound : public ASound
{
	GENERATED_BODY()
	
public:
	AEffectSound();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

};
