#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "C_FXComponent.generated.h"



USTRUCT( BlueprintType)
struct FFxInfo : public FTableRowBase
{
public:
	GENERATED_BODY()

public:
	class UNiagaraSystem* FX;
	float PlayRate;

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKSOUL_API UC_FXComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UC_FXComponent();

protected:
	virtual void BeginPlay() override;
		
private:
	void LoadAttackSkillInfoFromDataTable();

private:
	UFUNCTION()
		void StartAttachFXActivation(EWeaponType InEnum);

private:
	class AGameObject* OwnerObject;
	class UDA_Weapon* DataAsset;

private:
	TMap<EWeaponType, FFxInfo> MGhostEffects;

};
