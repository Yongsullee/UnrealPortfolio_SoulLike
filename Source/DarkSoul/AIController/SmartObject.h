#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "SmartObject.generated.h"

UCLASS()
class DARKSOUL_API ASmartObject : public AActor
{
	GENERATED_BODY()
	
public:	
	ASmartObject();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Comopnents")
		class UBillboardComponent* Billboard;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Comopnents")
		class UArrowComponent* FacingDirection;

};
