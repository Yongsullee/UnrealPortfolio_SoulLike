#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Portal.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FLevelChanged, TArray<class AWeapon*>, Weapons);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSoundFadeOut);

UCLASS()
class DARKSOUL_API APortal : public AActor
{
	GENERATED_BODY()
	
public:	
	APortal();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
		void SwitchLevel(class AActor* InActor);

private:
	void HandOverObjectCurrentState(class AActor** InActor);

public:
	FLevelChanged OnLevelChanged;
	FSoundFadeOut OnSoundFadeOut;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Compnents")
		class USphereComponent* Sphere;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class UNiagaraComponent* PortalEffect;

private:
	UPROPERTY(EditAnywhere, Category = "Level")
		FString NextLevelName;

private:
	const float SoundFadeOutDuration = 0.6f;

};
