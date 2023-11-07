#pragma once

#include "CoreMinimal.h"
#include "GameObjects/Enemy.h"

#include "CastleArchery.generated.h"

UCLASS()
class DARKSOUL_API ACastleArchery : public AEnemy
{
	GENERATED_BODY()

public:
	ACastleArchery();

protected:
	virtual void BeginPlay() override;

private:
	virtual void RestoreColor() override;
	virtual void LookAtDamageCauser(class ACharacter** InCharacter) override;

private:
	class UMaterialInstanceDynamic* BodyMaterial;

private:
	const EEnemyType EnemyType = EEnemyType::E_CastleArchery;

public:
	const EEnemyType GetEnemyType() const { return EnemyType; }
};
