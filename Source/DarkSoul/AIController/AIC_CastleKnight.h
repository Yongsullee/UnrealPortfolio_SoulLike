#pragma once

#include "CoreMinimal.h"
#include "AIC_EnemyBase.h"

#include "AIC_CastleKnight.generated.h"

UCLASS()
class DARKSOUL_API AAIC_CastleKnight : public AAIC_EnemyBase
{
	GENERATED_BODY()
	
public:
	AAIC_CastleKnight();

//	******************************************************************************************
			
	// Method
	 
//	******************************************************************************************

protected:
	virtual void BeginPlay() override;

private:
	virtual void OnPossess(class APawn* InPawn) override;

};
