#pragma once

#include "CoreMinimal.h"
#include "AIController/SmartObject.h"

#include "PatrolPath.generated.h"

UCLASS()
class DARKSOUL_API APatrolPath : public ASmartObject
{
	GENERATED_BODY()

public:
	APatrolPath();

protected:
	virtual void BeginPlay() override;

public:
	void GetSplinePoints();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Spline Path")
		class USplineComponent* Path;

	TArray<FVector> Locations;


};
