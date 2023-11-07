#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"

#include "EnvQueryContext_TargetEnemy.generated.h"

UCLASS()
class DARKSOUL_API UEnvQueryContext_TargetEnemy : public UEnvQueryContext
{
	GENERATED_BODY()

public:
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const;

};
