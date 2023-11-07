#pragma once

#include "CoreMinimal.h"
#include "GameObjects/BossEnemy.h"

#include "Demon.generated.h"

UCLASS()
class DARKSOUL_API ADemon : public ABossEnemy
{

	GENERATED_BODY()
	
public:
	ADemon();

protected:
	virtual void BeginPlay() override;

private:
	virtual void IsStartDamaged(class AActor* InDamageCauser, float InImpulseAmount = 0.f, bool bStatusEffectTriggered = false) override;
	virtual void IsEndDamaged() override;

private:
	virtual void ChangeColor() override;
	virtual void RestoreColor() override;

public:
	virtual void RunAICombatPhase(EAICombatPhase InEnum) override;

private:
	void AICombatPhase01();
	void AICombatPhase02();
	void AICombatPhase03();

private:
	const EEnemyType EnemyType = EEnemyType::E_Demon;

private:
	class UMaterialInstanceDynamic* BodyMaterial;
	class UMaterialInstanceDynamic* WingMaterial;

};
