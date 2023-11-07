#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "C_ManaComponent.generated.h"

DECLARE_DELEGATE_TwoParams(FUpdateHUDProfileManaBar, float, float);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FUpdateHUDProfileSkillCoolTime, int32, InSkillNum, float, InFloat1, float, InFloat2);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKSOUL_API UC_ManaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UC_ManaComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION()
		void RegenerateLeftMana();
	UFUNCTION()
		void DecreaseLeftMana(float InFloat);
	UFUNCTION()
		void RecoverLeftMana(float InRecoveryAmount);

public:
	UFUNCTION()
		void StartAttackSkillCoolTime(int32 InSkillNum, float InManaRequired, float InCoolTimeRequired);
	UFUNCTION()
		void AttackSkillCoolTimeCountDown();

	UFUNCTION()
		void EndAttackSkill00CoolTime();
	UFUNCTION()
		void EndAttackSkill01CoolTime();
	UFUNCTION()
		void EndAttackSkill02CoolTime();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Mana")
		float MaxMana = 100.f;
	UPROPERTY(EditDefaultsOnly, Category = "Mana")
		float LeftMana = 0.f;

	UPROPERTY(VisibleAnywhere, Category = "Mana")
		FTimerHandle RegenManaHandler;
	UPROPERTY(EditDefaultsOnly, Category = "Mana")
		float ManaRegenRate = 0.5f;

	TArray<bool> AttackSkillsValid;
	TArray<float> AttackSkillsCoolTimer;
	float StartTime[3];
	
public:
	FUpdateHUDProfileManaBar OnUpdateHUDProfileManaBar;
	FUpdateHUDProfileSkillCoolTime OnUpdateHUDProfileSkillCoolTime;

public:
	FORCEINLINE float GetLeftMana() { return LeftMana; }
	FORCEINLINE bool GetAttackSillValid(int32 InNum) { return AttackSkillsValid[InNum]; }
};
