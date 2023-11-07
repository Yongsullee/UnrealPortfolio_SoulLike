#include "C_PowerComponent.h"
#include "Components/CapsuleComponent.h"

#include "GameFramework/Character.h"
#include "Weapon/Weapon.h"
#include "GameFramework/DamageType.h"
#include "StatusEffects/StatusEffect.h"

#include "NiagaraFunctionLibrary.h"

UC_PowerComponent::UC_PowerComponent()
{
	
	PrimaryComponentTick.bCanEverTick = false;

	CHelpers::GetAsset(&DataTable_PowerInfo, "DataTable'/Game/DataTables/Statics/DataTable_PowerInfo'");

	// Impact Effect
	IsImpactEffectSocketNamesActive.Add(TTuple<FName, bool>(TEXT("ImpactEffect_L"), false));
	IsImpactEffectSocketNamesActive.Add(TTuple<FName, bool>(TEXT("ImpactEffect_R"), false));
	IsImpactEffectSocketNamesActive.Add(TTuple<FName, bool>(TEXT("ImpactEffect_Wing"), false));

	// #1. Holy -> Fire_Buff
	TSubclassOf<AStatusEffect> EffectClass; 
	CHelpers::GetClass(&EffectClass, "Blueprint'/Game/Blueprints/StatusEffects/BP_StatusEffect_Fire_Buff'");
	MStatusEffects.Add(EAttributeType::E_Holy, EffectClass);
	// #2. Dark -> Dark_Buff
	CHelpers::GetClass(&EffectClass, "Blueprint'/Game/Blueprints/StatusEffects/BP_StatusEffect_Dark_Buff'");
	MStatusEffects.Add(EAttributeType::E_Dark, EffectClass);

}

void UC_PowerComponent::BeginPlay()
{

	Super::BeginPlay();

	AActor* OwnerActor = GetOwner();
	OwnerWeapon = Cast<AWeapon>(OwnerActor);
	CheckTrue(!OwnerActor || !OwnerWeapon);

	LoadPowerInfoFromDataTable();

	OwnerWeapon->OnWeaponCollisionBeginOverlap.BindUFunction(this, TEXT("SendDamage"));

	// Remark, 03-17
	OwnerWeapon->OnBuffStatusEffect.AddDynamic(this, &UC_PowerComponent::TakeBuff);

}

// [23-04-27] : Load 함수
void UC_PowerComponent::LoadPowerInfoFromDataTable()
{

	if (!!DataTable_PowerInfo)
	{
		FString OwnerName = OwnerWeapon->GetName().Mid(3, OwnerWeapon->GetName().Len() - 7);
		CHelpers::LoadPowerInfoFromDataTable(&DataTable_PowerInfo, PowerInfo_Struct, OwnerName);
	}

}

/*	************************************************************************************

	[Remark]	:	Damage Type Class를 Power Component에서 직접 설정하도록 합니다.

	목적:
		Skill 공격일 경우, 각 무기의 AttributeType에 따라서, Status Effect를 Damage와 함께 전달합니다.
	설명:
		1. PowerComponent::DamageTypeClass를 Unreal Editor에서 설정하도록 합니다.
		2. Apply Damage 구현 과정에서 IsSkillDamage == true일 경우, 미리 설정한 Damage Type Class도 함께 전달합니다.

*	**********************************************************s**************************/

bool UC_PowerComponent::SendDamage(AActor* DamageCauser, AActor* OtherActor, bool IsSkillDamage, float SkillBasePower)
{

	AActor* WeaponOwnerActor= OwnerWeapon->GetOwner();
	if (!!WeaponOwnerActor)
	{
		const ACharacter* OwnerCharacter = Cast<ACharacter>(WeaponOwnerActor);
		if (!!OwnerCharacter)
		{
			TSubclassOf<UDamageType> DamageTypeClass;
			float FinalDamage = 0.f;
			// #0. Damage
			if (IsSkillDamage == true)
			{
				FinalDamage = (SkillBasePower * 0.5f) + (PowerInfo_Struct.AttackPower * 0.5f);
				DamageTypeClass = PowerInfo_Struct.DamageTypeClass;
			}
			else
			{
				FinalDamage = PowerInfo_Struct.AttackPower;
				DamageTypeClass = UDamageType::StaticClass();
			}

			// #1. Send Damage
			float damage = UGameplayStatics::ApplyDamage(
				OtherActor,
				FinalDamage,
				OwnerCharacter->GetInstigatorController(),
				GetOwner(),
				DamageTypeClass
			);

			// #2. Impact Effect Spawn
			SpawnImpactEffect(&OwnerCharacter);

			return damage > 0.f;

		}
		else
		{
			return false;
		}
	}
	else {
		return false;
	}

}

// Remark, 03-17
void UC_PowerComponent::TakeBuff()
{

	if (PowerInfo_Struct.DamageTypeClass)
	{
		UDamageType* DamageType = NewObject<UDamageType>(GetTransientPackage(), PowerInfo_Struct.DamageTypeClass);
		EAttributeType AttributeType = PowerInfo_Struct.Attribute;
		if (DamageType)
		{
			IDamageTypeHealthInterface* Interface = const_cast<IDamageTypeHealthInterface*>(Cast<IDamageTypeHealthInterface>(DamageType));
			if(Interface)
				Interface->TriggerBuffStatusEffect(this, AttributeType);
		}
		else
		{
			PrintLine();
			return;
		}

	}
	else
	{
		PrintLine();
		return;
	}

}

// Remark, 03-17
void UC_PowerComponent::TriggerStatusEffect(EAttributeType InAttributeType)
{

	if (OwnerWeapon)
	{
		AActor* OwnerWeaponOwner = OwnerWeapon->GetOwner();
		if (OwnerWeaponOwner)
		{
			ACharacter* OwnerCharacter = Cast<ACharacter>(OwnerWeaponOwner);
			if (OwnerCharacter)
			{
				if (MStatusEffects.Contains(InAttributeType))
				{
					TSubclassOf<AStatusEffect>* StatusEffectClass = nullptr;
					StatusEffectClass = MStatusEffects.Find(InAttributeType);
					if (StatusEffectClass && (*StatusEffectClass))
					{
						StatusEffect = GetWorld()->SpawnActor<AStatusEffect>((*StatusEffectClass));
						if (StatusEffect)
						{
							StatusEffect->SetOwner(OwnerCharacter);
							StatusEffect->AttachComponentToOwnerComponent(OwnerCharacter->GetMesh());

							StatusEffect->StartActivation();
						}
					}
				}
			}
		}
	}

}

void UC_PowerComponent::SpawnImpactEffect(const ACharacter** InCharacter)
{

	if (PowerInfo_Struct.ImpactEffect)
	{
		UNiagaraComponent* Effect = nullptr;

		for (uint8 i = 0; i < IsImpactEffectSocketNamesActive.Num(); i++)
		{
			if (IsImpactEffectSocketNamesActive.IsValidIndex(i) && IsImpactEffectSocketNamesActive[i].Get<1>())
			{
				const FName& SocketName = IsImpactEffectSocketNamesActive[i].Get<0>();
				USkeletalMeshComponent* CharacterMesh = (*InCharacter)->GetMesh();

				if (CharacterMesh && CharacterMesh->DoesSocketExist(SocketName))
				{
					Effect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
						(*InCharacter)->GetWorld(),
						PowerInfo_Struct.ImpactEffect,
						CharacterMesh->GetSocketLocation(SocketName),
						CharacterMesh->GetSocketRotation(SocketName)
					);
				}
			}
		}
	}


}
