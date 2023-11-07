#pragma once

#include "CoreMinimal.h"
#include "GameObject.h"
#include "Global/Global.h"

#include "EnemyBase.generated.h"

DECLARE_DYNAMIC_DELEGATE_ThreeParams(FEnemyIsDead, class AActor*, InDamageCauser, bool, IsBoss, uint8, InIndex);

UCLASS()
class DARKSOUL_API AEnemyBase : public AGameObject
{
	GENERATED_BODY()

public:
	AEnemyBase();

protected:
	virtual void BeginPlay() override;

// *****************************************************************************
		// METHOD
// *****************************************************************************

protected:
	virtual void IsStartDamaged(class AActor* InDamageCauser, float InImpulseAmount = 0.f, bool bStatusEffectTriggered = false) override;

private:
	UFUNCTION()
		void UpdateHUDProfileHealthBar(float InFloat1, float InFloat2);

protected:
	UFUNCTION()
		virtual void RunAICombatPhase(EAICombatPhase InEnum) override;

public:
	FEnemyIsDead OnEnemyIsDead;

// *****************************************************************************
		// Data Member
// *****************************************************************************

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Component")
		class UWidgetComponent* HUDComponent;

// *****************************************************************************
		// Setter&Getter
// *****************************************************************************
protected:
	struct PoolObjectInfo
	{
	public:
		uint8 Index = -1;
	}PoolObjectInfo_Struct;


public:
	FORCEINLINE void SetFaction(EFaction InType) { Faction = InType; }
	FORCEINLINE void SetIndex(uint8 InNum) { PoolObjectInfo_Struct.Index = InNum; }

};
