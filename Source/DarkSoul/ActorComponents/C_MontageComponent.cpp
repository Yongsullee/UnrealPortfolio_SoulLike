#include "C_MontageComponent.h"

#include "GameObjects/GameObject.h"
#include "Interface/OwnerWeaponInterface.h"

#include "DataAssets/DA_Weapon.h"

#include "ActorComponents/C_WeaponComponent.h"
#include "ActorComponents/C_VaultComponent.h"

UC_MontageComponent::UC_MontageComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

}


void UC_MontageComponent::BeginPlay()
{

	Super::BeginPlay();

	OwnerObject = Cast<AGameObject>(GetOwner());
	UC_VaultComponent* VaultComp = OwnerObject->FindComponentByClass<UC_VaultComponent>();
	DataAsset = OwnerObject->GetWeaponDataAsset();

	CheckTrue(!OwnerObject || !VaultComp || !DataAsset);

	LoadHitAnimInfoFromDataTable();
	LoadDeathAnimInfoFromDataTable();
	LoadKnockDownAnimInfoFromDataTable();
	LoadWakeUpAnimInfoFromDataTable();
	LoadComboAnimInfoFromDataTable();
	LoadAttackSkillAnimInfoFromDataTable();
	LoadGuardSkillAnimInfoFromDataTable();

	// #0. Hit + Death Anim
	OwnerObject->OnPlayHitAnim.BindUFunction(this, "PlayMontage");
	OwnerObject->OnPlayDeathAnim.BindUFunction(this, "PlayMontage");
	// [23-05-10] Remark : Knock Down Anim
	OwnerObject->OnPlayKnockDownAnim.BindUFunction(this, "PlayMontage");
	// [23-05-10] Remark : Wake Up Anim
	OwnerObject->OnPlayWakeUpAnim.BindUFunction(this, "PlayMontage");

	// #2. Vault - Double Jump
	VaultComp->OnPlayRollAnim.BindUFunction(this, "PlayMontage");
	VaultComp->OnPlayDoubleJumpAnim.BindUFunction(this, "PlayMontage");
	VaultComp->OnPlayParkourAnim.BindUFunction(this, "PlayMontage");
	VaultComp->OnPlayClimbAnim.BindUFunction(this, "PlayMontage");

}

void UC_MontageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{


	UC_WeaponComponent* WeaponComp = OwnerObject->FindComponentByClass<UC_WeaponComponent>();

	// #1. Attack + Attack Skill + Guard Skill + Guard Skill Block
	TArray<AWeapon*> Weapons = WeaponComp->GetWeapons();
	for (auto Weapon : Weapons)
	{
		if (Weapon)
			Weapon->OnPlayAnimMontage.BindUFunction(this, "PlayMontage");
	}

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UC_MontageComponent::LoadHitAnimInfoFromDataTable()
{

	if (!!OwnerObject && !!DataAsset)
	{
		UDataTable* HitInfo = DataAsset->GetHitInfo();
		CHelpers::LoadCommonAnimInfoFromDataTable(&HitInfo, MmHitAnims);
	}
	else
	{
		PrintLine();
		return;
	}

}

void UC_MontageComponent::LoadDeathAnimInfoFromDataTable()
{

	if (!!OwnerObject || !!DataAsset)
	{
		UDataTable* DeathInfo = DataAsset->GetDeathInfo();
		if (!!DeathInfo)
		{
			CHelpers::LoadCommonAnimInfoFromDataTable(&DeathInfo, MmDeathAnims);
		}
	}
	else
	{
		PrintLine();
		return;
	}

}

void UC_MontageComponent::LoadKnockDownAnimInfoFromDataTable()
{

	if (!!OwnerObject || !!DataAsset)
	{
		UDataTable* KnockDownInfo = DataAsset->GetKnockDownInfo();
		if (!!KnockDownInfo)
		{
			CHelpers::LoadCommonAnimInfoFromDataTable(&KnockDownInfo, MmKnockDownAnims);
		}
	}
	else
	{
		PrintLine();
		return;
	}

}

void UC_MontageComponent::LoadWakeUpAnimInfoFromDataTable()
{

	if (!!OwnerObject || !!DataAsset)
	{
		UDataTable* WakeUpInfo = DataAsset->GetWakeUpInfo();
		if (!!WakeUpInfo)
		{
			CHelpers::LoadCommonAnimInfoFromDataTable(&WakeUpInfo, MmWakeUpAnims);
		}
	}
	else
	{
		PrintLine();
		return;
	}

}

void UC_MontageComponent::LoadComboAnimInfoFromDataTable()
{

	if (!!OwnerObject && !!DataAsset)
	{
		TArray<UDataTable*> ComboType1Infos = DataAsset->GetComboType1_Infos();
		TArray<UDataTable*> ComboType2Infos = DataAsset->GetComboType2_Infos();
		CHelpers::LoadComboTypeInfoFromDataTable(&ComboType1Infos, MComboType1Anims);
		CHelpers::LoadComboTypeInfoFromDataTable(&ComboType2Infos, MComboType2Anims);
	}

}

void UC_MontageComponent::LoadAttackSkillAnimInfoFromDataTable()
{

	if (!!OwnerObject && !!DataAsset)
	{
		TArray<UDataTable*> AttackSkillInfos = DataAsset->GetAttackSkillInfos();
		CHelpers::LoadSkillInfoFromDataTable(&AttackSkillInfos, MAttackSkillAnims);
	}
	else
	{
		PrintLine();
		return;
	}

}

void UC_MontageComponent::LoadGuardSkillAnimInfoFromDataTable()
{

	if (!!OwnerObject && !!DataAsset)
	{
		TArray<UDataTable*> GuardSkillInfos = DataAsset->GetGuardSkillInfos();
		CHelpers::LoadSkillInfoFromDataTable(&GuardSkillInfos, MGuardSkillAnims);
	}
	else
	{
		PrintLine();
		return;
	}

}

void UC_MontageComponent::PlayMontage(EStateType InStateType, EWeaponType InWeaponType, EComboType InComboType, uint8 InComboCount, uint8 InSkillNum)
{

	switch (InStateType)
	{
	case EStateType::E_Stumbling :
		PlayHitAnimMontage(InWeaponType);
		break;
	case EStateType::E_Dead :
		PlayDeathAnimMontage(InWeaponType);
		break;
	case EStateType::E_KnockDown :
		PlayKnockDownAnimMontage(InWeaponType);
		break;
	case EStateType::E_WakeUp :
		PlayWakeUpAnimMontage(InWeaponType);
		break;
	case EStateType::E_Attacking :
		PlayAttackAnimMontage(InWeaponType, InComboType, InComboCount);
		break;
	case EStateType::E_Skilling:
		PlayAttackSkillAnimMontage(InWeaponType, InSkillNum);
		break;
	case EStateType::E_Guarding:
		PlayGuardSkillAnimMontage(InWeaponType, InSkillNum);
		break;
	case EStateType::E_Rolling:
		PlayRollAnimMontage();
		break;
	case EStateType::E_Vaulting_DoubleJump:
		PlayDoubleJumpAnimMontage();
		break;
	case EStateType::E_Vaulting_Parkour:
		PlayParkourAnimMontage();
		break;
	case EStateType::E_Vaulting_Climb:
		PlayClimbAnimMontage();
		break;
	}

}

void UC_MontageComponent::PlayHitAnimMontage(const EWeaponType& InWeaponType)
{

	if ((MmHitAnims.Num(InWeaponType) <= 0) || !OwnerObject)
	{
		PrintLine();
		return;
	}

	if (MmHitAnims.Contains(InWeaponType))
	{
		int8 Index = -1;
		if (MmHitAnims.Num(InWeaponType) > 1)
		{
			// #1. Random
			Index = FMath::RandRange(0, MmHitAnims.Num(InWeaponType) - 1);
		}
		else
		{
			// #2. Not Random
			Index = 0;
		}

		TArray<FCommonAnimInfo> HitAnimInfos;
		MmHitAnims.MultiFind(InWeaponType, HitAnimInfos);

		if (HitAnimInfos.IsValidIndex(Index))
		{
			FCommonAnimInfo HitAnimInfo = HitAnimInfos[Index];
			if (HitAnimInfo.AnimMontage)
			{
				OwnerObject->PlayAnimMontage(
					HitAnimInfo.AnimMontage, 
					HitAnimInfo.PlayRate,
					HitAnimInfo.StartSection
				);
			}
		}

	}

}

void UC_MontageComponent::PlayDeathAnimMontage(const EWeaponType& InWeaponType)
{

	CheckTrue((MmDeathAnims.Num(InWeaponType) <= 0) || !OwnerObject);

	if (MmDeathAnims.Num(InWeaponType) > 1)
	{
		TArray<FCommonAnimInfo> DeathAnimInfos;
		MmDeathAnims.MultiFind(InWeaponType, DeathAnimInfos);

		FCommonAnimInfo DeathAnimInfo = DeathAnimInfos[FMath::RandRange(0, MmDeathAnims.Num(InWeaponType) - 1)];

		OwnerObject->PlayAnimMontage(
			DeathAnimInfo.AnimMontage, 
			DeathAnimInfo.PlayRate,
			DeathAnimInfo.StartSection
		);
	}
	else
	{
		OwnerObject->PlayAnimMontage(
			MmDeathAnims.Find(InWeaponType)->AnimMontage,
			MmDeathAnims.Find(InWeaponType)->PlayRate,
			MmDeathAnims.Find(InWeaponType)->StartSection
		);
	}

}

// [23-05-20] Remark : Knock Down Anims
void UC_MontageComponent::PlayKnockDownAnimMontage(const EWeaponType& InWeaponType)
{

	if ((MmKnockDownAnims.Num(InWeaponType) <= 0) || !OwnerObject)
	{
		PrintLine();
		return;
	}

	TArray<FCommonAnimInfo> KnockDownInfos;
	MmKnockDownAnims.MultiFind(InWeaponType, KnockDownInfos);

	if (KnockDownInfos.Num() > 1)
	{
		int8 Index = FMath::RandRange(0, KnockDownInfos.Num() - 1);
		if (KnockDownInfos.IsValidIndex(Index))
		{
			FCommonAnimInfo KnockDownInfo = KnockDownInfos[Index];
			if (KnockDownInfo.AnimMontage)
			{
				OwnerObject->PlayAnimMontage(
					KnockDownInfo.AnimMontage,
					KnockDownInfo.PlayRate,
					KnockDownInfo.StartSection
				);
			}
		}
	}
	else if (KnockDownInfos.Num() == 1)
	{
		if (KnockDownInfos.IsValidIndex(0))
		{
			FCommonAnimInfo KnockDownInfo = KnockDownInfos[0];
			if (KnockDownInfo.AnimMontage)
			{
				OwnerObject->PlayAnimMontage(
					KnockDownInfo.AnimMontage,
					KnockDownInfo.PlayRate,
					KnockDownInfo.StartSection
				);
			}
		}
	}
}

// [23-05-10] Remark : Wake Up Anim
void UC_MontageComponent::PlayWakeUpAnimMontage(const EWeaponType& InWeaponType)
{

	CheckTrue((MmWakeUpAnims.Num(InWeaponType) <= 0) || !OwnerObject);

	if (MmWakeUpAnims.Num(InWeaponType) > 1)
	{
		TArray<FCommonAnimInfo> WakeUpInfos;
		MmWakeUpAnims.MultiFind(InWeaponType, WakeUpInfos);

		FCommonAnimInfo WakeUpInfo = WakeUpInfos[FMath::RandRange(0, MmWakeUpAnims.Num(InWeaponType) - 1)];

		OwnerObject->PlayAnimMontage(
			WakeUpInfo.AnimMontage,
			WakeUpInfo.PlayRate,
			WakeUpInfo.StartSection
		);
	}
	else
	{
		OwnerObject->PlayAnimMontage(
			MmWakeUpAnims.Find(InWeaponType)->AnimMontage,
			MmWakeUpAnims.Find(InWeaponType)->PlayRate,
			MmWakeUpAnims.Find(InWeaponType)->StartSection
		);
	}

}

void UC_MontageComponent::PlayAttackAnimMontage(const EWeaponType& InWeaponType, const EComboType& InComboType, uint8 InComboCount)
{

	CheckTrue(!OwnerObject);

	TMap<EWeaponType, TArray<FComboAnimInfo*>>* AnimationsMap = nullptr;
	switch (InComboType)
	{
	case EComboType::E_NoCombo:
		AnimationsMap = &MComboType1Anims;
		break;
	case EComboType::E_ComboType_1:
		AnimationsMap = &MComboType1Anims;
		break;
	case EComboType::E_ComboType_2:
		AnimationsMap = &MComboType2Anims;
		break;
	case EComboType::E_Max:
	default:
		return; // No valid combo type provided.
	}

	if (AnimationsMap && AnimationsMap->Contains(InWeaponType))
	{
		TArray<FComboAnimInfo*>* Combos = AnimationsMap->Find(InWeaponType);
		if ((*Combos).Num() > 0)
		{
			uint8 Index = InComboType == EComboType::E_NoCombo ? FMath::RandRange(0, (*Combos).Num() - 1) : InComboCount;

			if ((*Combos).IsValidIndex(Index))
			{
				FComboAnimInfo* CombatAnimInfo = (*Combos)[Index];
				if (CombatAnimInfo->AnimMontage)
				{
					OwnerObject->PlayAnimMontage(
						(*(*Combos)[Index]).AnimMontage,
						(*(*Combos)[Index]).PlayRate,
						(*(*Combos)[Index]).StartSection
					);
				}
			}
		}
	}

}


void UC_MontageComponent::PlayAttackSkillAnimMontage(const EWeaponType& InWeaponType, uint8 InSkillNum)
{

	TArray<FSkillAnimInfo*>* AttackSkillAnimStructs;

	if (MAttackSkillAnims.Contains(InWeaponType))
	{
		AttackSkillAnimStructs = MAttackSkillAnims.Find(InWeaponType);
		if (AttackSkillAnimStructs->IsValidIndex(InSkillNum))
		{
			FSkillAnimInfo* SkillAnimInfo = (*AttackSkillAnimStructs)[InSkillNum];
			if (OwnerObject && SkillAnimInfo->AnimMontage)
			{
				OwnerObject->PlayAnimMontage(
					SkillAnimInfo->AnimMontage,
					SkillAnimInfo->PlayRate,
					SkillAnimInfo->StartSection
				);
			}

		}
	}

}

void UC_MontageComponent::PlayGuardSkillAnimMontage(const EWeaponType& InWeaponType, uint8 InSkillNum)
{

	TArray<FSkillAnimInfo*>* GuardSkillAnimStructs;

	if (MGuardSkillAnims.Contains(InWeaponType))
	{
		GuardSkillAnimStructs = MGuardSkillAnims.Find(InWeaponType);
		if (GuardSkillAnimStructs->IsValidIndex(InSkillNum))
		{
			FSkillAnimInfo* SkillAnimInfo = (*GuardSkillAnimStructs)[InSkillNum];
			if (OwnerObject && SkillAnimInfo->AnimMontage)
			{
				OwnerObject->PlayAnimMontage(
					SkillAnimInfo->AnimMontage,
					SkillAnimInfo->PlayRate,
					SkillAnimInfo->StartSection
				);
			}

		}
	}
	
}

void UC_MontageComponent::PlayRollAnimMontage()
{

	CheckTrue(!OwnerObject || !RollAnimMontage);

	OwnerObject->PlayAnimMontage(RollAnimMontage);

}

void UC_MontageComponent::PlayDoubleJumpAnimMontage()
{

	CheckTrue(!OwnerObject || !DoubleJumpAnimMontage);

	OwnerObject->PlayAnimMontage(DoubleJumpAnimMontage);

}

void UC_MontageComponent::PlayParkourAnimMontage()
{

	CheckTrue(!OwnerObject || !ParkourAnimMontages.IsValidIndex(0));

	uint8 Index = FMath::RandRange(0, ParkourAnimMontages.Num() - 1);

	OwnerObject->PlayAnimMontage(ParkourAnimMontages[Index], 1.3f);

}

void UC_MontageComponent::PlayClimbAnimMontage()
{

	CheckTrue(!OwnerObject || !ClimbAnimMontage);

	OwnerObject->PlayAnimMontage(ClimbAnimMontage, 1.3f);

}

