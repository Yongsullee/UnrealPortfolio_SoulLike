#include "C_ManaComponent.h"
#include "Global/Global.h"

#include "ActorComponents/C_WeaponComponent.h"

UC_ManaComponent::UC_ManaComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	LeftMana = MaxMana;
	//LeftMana = 10.f;

	for (int32 i = 0; i < 3; i++)
	{
		AttackSkillsValid.Add(true);
		AttackSkillsCoolTimer.Add(0.f);
		StartTime[i] = 0.f;
	}

}


void UC_ManaComponent::BeginPlay()
{

	Super::BeginPlay();

	AActor* OwnerActor = GetOwner();
	UC_WeaponComponent* OwnerWeaponComp = OwnerActor->FindComponentByClass<UC_WeaponComponent>();
	
	CheckTrue(!OwnerActor || !OwnerWeaponComp);

	GetWorld()->GetTimerManager().SetTimer(RegenManaHandler, this, &UC_ManaComponent::RegenerateLeftMana, 1.f, true);

	OwnerWeaponComp->OnRequireMana.BindUFunction(this, "StartAttackSkillCoolTime");

}

void UC_ManaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 0. Cool Timer Countdown
	AttackSkillCoolTimeCountDown();

}



void UC_ManaComponent::RegenerateLeftMana()
{
	LeftMana = FMath::Clamp(LeftMana + ManaRegenRate, 0.f, MaxMana);

	OnUpdateHUDProfileManaBar.ExecuteIfBound(MaxMana, LeftMana);
}

void UC_ManaComponent::DecreaseLeftMana(float InFloat)
{

	LeftMana = FMath::Clamp(LeftMana - InFloat, 0.f, MaxMana);

	OnUpdateHUDProfileManaBar.ExecuteIfBound(MaxMana, LeftMana);
}

void UC_ManaComponent::RecoverLeftMana(float InRecoveryAmount)
{
	LeftMana = FMath::Clamp(LeftMana + InRecoveryAmount, 0.f, MaxMana);

	OnUpdateHUDProfileManaBar.ExecuteIfBound(MaxMana, LeftMana);
}

void UC_ManaComponent::StartAttackSkillCoolTime(int32 InSkillNum, float InManaRequired, float InCoolTimeRequired)
{
	
	// TODO	:	Mana Decrease
	DecreaseLeftMana(InManaRequired);

	// TODO	:	Attack Skill Is Not Valid Now
	AttackSkillsValid[InSkillNum] = false;

	// TODO	:	Cool Timer CountDown Start
	AttackSkillsCoolTimer[InSkillNum] = InCoolTimeRequired;
	StartTime[InSkillNum] = GetWorld()->GetTimeSeconds();

	// TODO	:	Cool Time
	FTimerHandle CoolTimeHandle;
	float CoolTimeRate = InCoolTimeRequired;
	switch (InSkillNum)
	{
	case 0:
		GetWorld()->GetTimerManager().SetTimer(CoolTimeHandle, FTimerDelegate::CreateLambda([&]() {
			EndAttackSkill00CoolTime();
			}), CoolTimeRate, false);
		break;
	case 1:
		GetWorld()->GetTimerManager().SetTimer(CoolTimeHandle, FTimerDelegate::CreateLambda([&]() {
			EndAttackSkill01CoolTime();
			}), CoolTimeRate, false);
		break;
	case 2:
		GetWorld()->GetTimerManager().SetTimer(CoolTimeHandle, FTimerDelegate::CreateLambda([&]() {
			EndAttackSkill02CoolTime();
			}), CoolTimeRate, false);
		break;
	}
	
}

void UC_ManaComponent::AttackSkillCoolTimeCountDown()
{

	for (int32 i = 0; i < AttackSkillsCoolTimer.Num(); i++)
	{
		if (AttackSkillsValid[i] == false)
		{
			float CoolTime = AttackSkillsCoolTimer[i];

			float CoolTimeCountDown = CoolTime - fabs(StartTime[i] - GetWorld()->GetTimeSeconds());
			CoolTimeCountDown = FMath::Max(CoolTimeCountDown, 0.f);
			
			if (OnUpdateHUDProfileSkillCoolTime.IsBound()) OnUpdateHUDProfileSkillCoolTime.Broadcast(i, CoolTime, CoolTimeCountDown);
		}
	}
}


void UC_ManaComponent::EndAttackSkill00CoolTime()
{

	CheckNull(AttackSkillsValid.IsValidIndex(0));

	AttackSkillsValid[0] = true;
	AttackSkillsCoolTimer[0] = 0.f;
	StartTime[0] = 0.f;

}

void UC_ManaComponent::EndAttackSkill01CoolTime()
{

	CheckNull(AttackSkillsValid.IsValidIndex(1));
	
	AttackSkillsValid[1] = true;
	AttackSkillsCoolTimer[1] = 0.f;
	StartTime[1] = 0.f;


}

void UC_ManaComponent::EndAttackSkill02CoolTime()
{

	CheckNull(AttackSkillsValid.IsValidIndex(2));
	
	AttackSkillsValid[2] = true;
	AttackSkillsCoolTimer[2] = 0.f;
	StartTime[2] = 0.f;

}
