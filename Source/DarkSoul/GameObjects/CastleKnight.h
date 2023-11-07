#pragma once

#include "CoreMinimal.h"
#include "GameObjects/Enemy.h"

#include "CastleKnight.generated.h"

UCLASS()
class DARKSOUL_API ACastleKnight : public AEnemy
{
	GENERATED_BODY()
public:

	ACastleKnight();

protected:
	virtual void BeginPlay() override;

private:
	virtual void RestoreColor() override;
	virtual void LookAtDamageCauser(class ACharacter** InCharacter) override;


private:
	class UMaterialInstanceDynamic* BodyMaterial;

private:
	const EEnemyType EnemyType = EEnemyType::E_CastleKnight;

public:
	const EEnemyType GetEnemyType() const { return EnemyType; }

};
