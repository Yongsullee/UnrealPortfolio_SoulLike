#include "C_HealthComponent.h"
#include "Global/Global.h"

#include "GameObjects/GameObject.h"
#include "ActorComponents/C_StateComponent.h"

#include "StatusEffects/StatusEffect.h"
#include "Interface/DamageTypeHealthInterface.h"


UC_HealthComponent::UC_HealthComponent()
{

	PrimaryComponentTick.bCanEverTick = false;

	CHelpers::GetAsset(&DataTable_SpecInfo, "DataTable'/Game/DataTables/Statics/DataTable_SpecInfo'");

	TSubclassOf<AStatusEffect> EffectClass;
	// Burn Effect
	CHelpers::GetClass(&EffectClass, "Blueprint'/Game/Blueprints/StatusEffects/BP_StatusEffect_Burn'");
	MStatusEffects.Add(EAttributeType::E_Fire, EffectClass);
	// Frozen Effect
	CHelpers::GetClass(&EffectClass, "Blueprint'/Game/Blueprints/StatusEffects/BP_StatusEffect_Frozen'");
	MStatusEffects.Add(EAttributeType::E_Ice, EffectClass);

	// Electric Shock Effect
	CHelpers::GetClass(&EffectClass, "Blueprint'/Game/Blueprints/StatusEffects/BP_StatusEffect_ElectricShock'");
	MStatusEffects.Add(EAttributeType::E_Electric, EffectClass);

	// Threat Effect
	CHelpers::GetClass(&EffectClass, "Blueprint'/Game/Blueprints/StatusEffects/BP_StatusEffect_Threat'");
	MStatusEffects.Add(EAttributeType::E_Dark, EffectClass);

	// Debuff Effect
	CHelpers::GetClass(&EffectClass, "Blueprint'/Game/Blueprints/StatusEffects/BP_StatusEffect_Debuff'");
	MStatusEffects.Add(EAttributeType::E_Steel, EffectClass);


}

void UC_HealthComponent::BeginPlay()
{

	Super::BeginPlay();

	AActor* OwnerActor = GetOwner();
	OwnerObject = Cast<AGameObject>(OwnerActor);
	CheckTrue(!OwnerActor || !OwnerObject);

	// #1 Stat
	LoadSpecInfoFromDataTable();

	CurrentHealth = SpecInfo_Struct.MaxHealth;

	// #2 Regenerate
	GetWorld()->GetTimerManager().SetTimer(RegenHealthHandler, this, &UC_HealthComponent::RegenerateCurrentHealth, 1.f, true);

	// #3 Damage Delegate
	OwnerObject->OnTakeAnyDamage.AddDynamic(this, &UC_HealthComponent::TakeDamage);

}


void UC_HealthComponent::LoadSpecInfoFromDataTable()
{

	if (!!DataTable_SpecInfo)
	{
		FString OwnerName = OwnerObject->GetName();
		CHelpers::LoadSpecInfoFromDataTable(&DataTable_SpecInfo, SpecInfo_Struct, OwnerName);
	}

}

/*	************************************************************************************************************************
	
	[Remark]	:	Weapon�� �⺻ ������ ���ް� Status Effect�� ������ ������ �����մϴ�.

	����:
		Weapon�� �����ϴ� �⺻ Damage�� Status Effect�� �����ϴ� �������� �����մϴ�.
	����:
		1. Damage Causer�� Weapon �� ��� Damage Causer�� Status Effect�� ���� �����ϴ�.
		2. Daamge Causer�� Weapon �� ���, Owner Actor�� Guard Block + Hit Anim Play + Trigger Status Effect�� �����մϴ�.
		3. Damage Causer�� Status Effect�� ���, �� Status Effect�� ���� ������ ������ ���� ����� �����մϴ�.
		
*	************************************************************************************************************************/

void UC_HealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{

	CheckTrue(Damage <= 0.f || CurrentHealth < 0.f);

	float FinalDamage = FMath::Max(Damage - (SpecInfo_Struct.DefensePower * 0.4f), 0.f);
	bool bStatusEffectTriggered = false;

	if (!!DamageCauser)
	{
		AWeapon* WeaponCauser = Cast<AWeapon>(DamageCauser);
		if (WeaponCauser)
		{
			if (HandleBlockDamage(WeaponCauser) == true)
			{
				return;
			}

			IDamageTypeHealthInterface* Interface = const_cast<IDamageTypeHealthInterface*>(Cast<IDamageTypeHealthInterface>(DamageType));
			if (Interface)
			{
				Interface->TriggerDebuffStatusEffect(this);
				bStatusEffectTriggered = true;
			}

			DecreaseCurrentHealth(FinalDamage);

			HandleDamage(WeaponCauser, bStatusEffectTriggered);

		}
		else
		{
			AStatusEffect* StatusEffectCauser = Cast<AStatusEffect>(DamageCauser);
			if (StatusEffect)
			{
				DecreaseCurrentHealth(FinalDamage);

				HandleDamage(StatusEffect, bStatusEffectTriggered);
			}
		}
		
	}

	
}


/*	************************************************************************************

	[Remark]	:	FUpdateHUDProfileHealthBar�� ���ε��� �ܺ� Ŭ������ �Լ��� ȣ���մϴ�.

	����:
		Damage�� ���� Player Character�� HUD�� Health Bar�� Update�ϱ� �����Դϴ�.

*	************************************************************************************/

void UC_HealthComponent::RegenerateCurrentHealth()
{
	
	CurrentHealth = FMath::Clamp(CurrentHealth + HealthRegenRate, 0.f, SpecInfo_Struct.MaxHealth);

	OnUpdateHUDProfileHealthBar.ExecuteIfBound(SpecInfo_Struct.MaxHealth, CurrentHealth);
	
}

void UC_HealthComponent::RecoverCurrentHealth(float InRecoveryAmount)
{
	
	CurrentHealth = FMath::Clamp(CurrentHealth + InRecoveryAmount, 0.f, SpecInfo_Struct.MaxHealth);

	// HUD Udpate
	OnUpdateHUDProfileHealthBar.ExecuteIfBound(SpecInfo_Struct.MaxHealth, CurrentHealth);
	OnUpdateEnemyHUDProfileHealthBar.ExecuteIfBound(SpecInfo_Struct.MaxHealth, CurrentHealth);

}


void UC_HealthComponent::DecreaseCurrentHealth(float InFloat)
{

	CurrentHealth = FMath::Clamp(CurrentHealth - InFloat, 0.f, SpecInfo_Struct.MaxHealth);

	// HUD Udpate
	OnUpdateHUDProfileHealthBar.ExecuteIfBound(SpecInfo_Struct.MaxHealth, CurrentHealth);
	OnUpdateEnemyHUDProfileHealthBar.ExecuteIfBound(SpecInfo_Struct.MaxHealth, CurrentHealth);

}

bool UC_HealthComponent::HandleBlockDamage(AActor* InActor)
{

	if (OwnerObject == nullptr) return false;

	FVector DamageCauserDirection = InActor->GetActorForwardVector();
	FVector DamagedActorDirection = OwnerObject->GetActorForwardVector();

	float DotProduct = UKismetMathLibrary::DotProduct2D(FVector2D(DamageCauserDirection), FVector2D(DamagedActorDirection));

	if (DotProduct < 0.f && OwnerObject->GetStateComponent()->GetStateType() == EStateType::E_Guarding)
	{
		OnStartGuardSkillBlock.ExecuteIfBound();
		return true;
	}
	else
	{
		return false;
	}

}

// [23-08-10] : Status Effect ���� 
void UC_HealthComponent::HandleDamage(AActor* InActor, bool bStatusEffectTriggered)
{


	if (InActor)
	{
		int32 ImpulseAmount = -1;
		AWeapon* Weapon = Cast<AWeapon>(InActor);
		// #1. Impulse Amount
		if (Weapon)
		{
			IDamageTypeHealthInterface* Interface = const_cast<IDamageTypeHealthInterface*>(Cast<IDamageTypeHealthInterface>(Weapon));
			if (Interface)
			{
				ImpulseAmount = Interface->GetImpulseAmount();
			}
		}
		else 
		{
			if(StatusEffect)
				ImpulseAmount = 0;
		}

		// #2. Handle Damage
		if (ImpulseAmount != -1)
		{
			// #1. Damage
			if (CurrentHealth > 0.f)
			{
				OnOwnerIsDamaged.ExecuteIfBound(InActor, ImpulseAmount, bStatusEffectTriggered);
			}
			// #2. Dead 
			else
			{
				OnOwnerIsDead.ExecuteIfBound(InActor, ImpulseAmount, bStatusEffectTriggered);

				FTimerHandle DropHandler;
				GetWorld()->GetTimerManager().SetTimer(DropHandler, FTimerDelegate::CreateLambda([&]() {
					OnDropRewardItem.ExecuteIfBound();
					}), 1.5f, false);

				if (StatusEffect)
				{
					StatusEffect->EndActivation();
				}
			}
		}
	}

}

void UC_HealthComponent::TriggerStatusEffect(EAttributeType InAttributeType)
{

	CheckTrue(!OwnerObject || !MStatusEffects.Find(InAttributeType)); 

	StatusEffect = GetWorld()->SpawnActor<AStatusEffect>(*(MStatusEffects.Find(InAttributeType)));
	if (!!StatusEffect)
	{
		StatusEffect->SetOwner(OwnerObject);	
		StatusEffect->AttachComponentToOwnerComponent(OwnerObject->GetMesh());
		StatusEffect->StartActivation();
	}

}

