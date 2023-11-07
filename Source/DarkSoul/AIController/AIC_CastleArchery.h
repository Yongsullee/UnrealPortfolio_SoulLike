#pragma once

#include "CoreMinimal.h"
#include "AIC_EnemyBase.h"

#include "AIC_CastleArchery.generated.h"

UCLASS()
class DARKSOUL_API AAIC_CastleArchery : public AAIC_EnemyBase
{
	GENERATED_BODY()
	
// ******************************************************************************************

		// Method

//	******************************************************************************************

public:
	AAIC_CastleArchery();

protected:
	virtual void BeginPlay() override;

private:
	virtual void OnPossess(class APawn* InPawn) override;

};
