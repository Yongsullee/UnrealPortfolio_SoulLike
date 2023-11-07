#include "C_WeaponComponent.h"
#include "Global/Global.h"

#include "GameObjects/GameObject.h"
#include "Weapon/Weapon.h"
#include "DataAssets/DA_Weapon.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "ActorComponents/C_ManaComponent.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"


UC_WeaponComponent::UC_WeaponComponent()
{

	PrimaryComponentTick.bCanEverTick = false;

}

void UC_WeaponComponent::BeginPlay()
{

	Super::BeginPlay();

	OwnerObject = Cast<AGameObject>(GetOwner());
	DataAsset = OwnerObject->GetWeaponDataAsset();
	CheckTrue(!OwnerObject || !DataAsset);

	// #0. Spawn Weapons
	SpawnAttachWeapon();

	// #1. Get Weapon Infos
	LoadComboAnimInfoFromDataTable();
	LoadSkillAnimInfoFromDataTable();

	// #2. Equip Weapon
	//if (Weapons.Num() > 0)
	//{
	//	EWeaponType Type = Weapons[0]->GetWeaponType();
	//	EquipWeapon(Type);
	//}

	// #2. Delegates
	// #2-1 Change Weapon
	OwnerObject->OnChangeWeapon.BindUFunction(this, "ChangeWeapon");

	// #2-2. Weapon Attack
	OwnerObject->OnWeaponAttack.AddDynamic(this, &UC_WeaponComponent::Attack);
	OwnerObject->OnWeaponEndAttack.AddDynamic(this, &UC_WeaponComponent::EndAttack);

	// #2-3. Weapon Attack Skill
	OwnerObject->OnWeaponAttackSkill.AddDynamic(this, &UC_WeaponComponent::AttackSkill);
	OwnerObject->OnWeaponEndAttackSkill.AddDynamic(this, &UC_WeaponComponent::EndAttackSkill);

	// #2-4. Weapon Guard Skill
	OwnerObject->OnWeaponGuardSkill.AddDynamic(this, &UC_WeaponComponent::GuardSkill);
	OwnerObject->OnWeaponGuardSkill_Block.AddDynamic(this, &UC_WeaponComponent::GuardSkill_Block);
	OwnerObject->OnWeaponEndGuardSkill.AddDynamic(this, &UC_WeaponComponent::EndGuardSkill);

}

void UC_WeaponComponent::LoadComboAnimInfoFromDataTable()
{

	if (!!OwnerObject && !!DataAsset)
	{
		TArray<UDataTable*> ComboType1DataTables = DataAsset->GetComboType1_Infos();
		TArray<UDataTable*> ComboType2DataTables = DataAsset->GetComboType2_Infos();

		CHelpers::LoadComboTypeInfoFromDataTable(&ComboType1DataTables, MComboType1Structs);
		CHelpers::LoadComboTypeInfoFromDataTable(&ComboType2DataTables, MComboType2Structs);
	}

}

void UC_WeaponComponent::LoadSkillAnimInfoFromDataTable()
{

	if (!!OwnerObject && !!DataAsset)
	{
		TArray<UDataTable*> AttackSkillDataTables = DataAsset->GetAttackSkillInfos();
		TArray<UDataTable*> GuardSkillDataTables = DataAsset->GetGuardSkillInfos();

		CHelpers::LoadSkillInfoFromDataTable(&AttackSkillDataTables, MAttackSkillStructs);
		CHelpers::LoadSkillInfoFromDataTable(&GuardSkillDataTables, MGuardSkillStructs);
	}

}

void UC_WeaponComponent::SpawnAttachWeapon()
{

	if (OwnerObject && DataAsset)
	{
		TArray <TSubclassOf<AWeapon>> WeaponClasses = DataAsset->GetWeaponClasses();
		for (const auto& WeaponClass : WeaponClasses)
		{
			AWeapon* Weapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass);
			if (!!Weapon)
			{
				Weapon->AttachComponentToOwnerComponent(OwnerObject->GetMesh());
				Weapon->SetOwner(OwnerObject);
				Weapon->OffSelected();
				Weapon->OffCollision();
				Weapons.Add(Weapon);
			}
			else
			{
				PrintLine();
				return;
			}
		}
	}

}

void UC_WeaponComponent::ChangeWeapon(EWeaponType InType)
{

	CheckTrue(!OwnerObject);

	if (WeaponType == EWeaponType::E_Unarmed && InType != EWeaponType::E_Unarmed)
		EquipWeapon(InType);
	else if (WeaponType == InType)
		UnequipWeapon(InType);
	else if (WeaponType != EWeaponType::E_Unarmed && InType != EWeaponType::E_Unarmed)
		InterchangeWeapon(WeaponType, InType);

}

void UC_WeaponComponent::ChangeComboType(EComboType InType)
{

	CheckTrue(!CurrentWeapon || CurrentWeapon->GetComboType() == InType || CurrentWeapon->GetComboType() == EComboType::E_NoCombo);

	CurrentWeapon->SetComboType(InType);

}

/*	************************************************************

	[Remark]	:	Weapon 장착

	목적:
		WeaponType을 통해 장착할 Weapon을 결정합니다.

*	************************************************************/

void UC_WeaponComponent::EquipWeapon(EWeaponType& InType)
{

	CheckTrue(!OwnerObject);

	for (const auto& Weapon : Weapons)
		if (Weapon->GetWeaponType() == InType) CurrentWeapon = Weapon;
	CurrentWeapon->OnSelected();

	WeaponType = InType;

	OwnerObject->bUseControllerRotationYaw = true;
	OwnerObject->GetCharacterMovement()->bOrientRotationToMovement = false;

	UpdateHUDProfile();

}

/*	************************************************************

	[Remark]	:	Weapon 해제

	목적:
		WeaponType을 Unarmed 로 변경하고, 현재 무기를 해제합니다.

*	************************************************************/

void UC_WeaponComponent::UnequipWeapon(EWeaponType& InType)
{

	CheckTrue(!OwnerObject);

	CurrentWeapon->OffSelected();
	CurrentWeapon = nullptr;

	WeaponType = EWeaponType::E_Unarmed;

	OwnerObject->bUseControllerRotationYaw = false;
	OwnerObject->GetCharacterMovement()->bOrientRotationToMovement = true;

	UpdateHUDProfile();

}

/*	************************************************************
	
	[Remark]	:	Weapon 교체

	목적:
		WeaponType을 통해 현재 무기와 새로 장착할 무기를 교체합니다.

*	************************************************************/

void UC_WeaponComponent::InterchangeWeapon(EWeaponType& InType1, EWeaponType& InType2)
{

	// 1. 현재 무기 해제
	UnequipWeapon(InType1);
	// 2. 다음 무기 장착
	EquipWeapon(InType2);

}

void UC_WeaponComponent::UpdateHUDProfile()
{

	if (!!CurrentWeapon)
	{
		FWeaponTextureInfo WeaponTextureInfo = CurrentWeapon->GetWeaponTextureInfoStruct();
		TArray<FSkillAnimInfo*>* SkillInfos = MAttackSkillStructs.Find(CurrentWeapon->GetWeaponType());

		OnUpdateHUDWeaponImage.ExecuteIfBound(
			WeaponTextureInfo.WeaponImage ? WeaponTextureInfo.WeaponImage : BlankImage);

		if (SkillInfos)
		{
			if (SkillInfos->Num() >= 3)
			{
				OnUpdateHUDSkillIcons.ExecuteIfBound(
					!!(*(*SkillInfos)[0]).SkillIcon ? (*(*SkillInfos)[0]).SkillIcon : BlankImage,
					!!(*(*SkillInfos)[1]).SkillIcon ? (*(*SkillInfos)[1]).SkillIcon : BlankImage,
					!!(*(*SkillInfos)[2]).SkillIcon ? (*(*SkillInfos)[2]).SkillIcon : BlankImage);
			}
		}
	}
	else
	{
		OnUpdateHUDWeaponImage.ExecuteIfBound(BlankImage);
		OnUpdateHUDSkillIcons.ExecuteIfBound(
			BlankImage,
			BlankImage,
			BlankImage);
	}

}

void UC_WeaponComponent::SwapWeapon(EWeaponType InType, AWeapon* InWeapon)
{
	
	for (auto& Weapon : Weapons)
	{
		if (Weapon->GetWeaponType() == InType)
		{
			Weapon->OffSelected();

			InWeapon->AttachComponentToOwnerComponent(OwnerObject->GetMesh());
			InWeapon->SetOwner(OwnerObject);
			InWeapon->OffCollision();
			InWeapon->OffSelected();

			if (CurrentWeapon == Weapon)
			{
				InWeapon->OnSelected();
				CurrentWeapon = InWeapon;

				UpdateHUDProfile();
			}

			Weapon = InWeapon;
		}
	}

}

void UC_WeaponComponent::Attack()
{

	CheckTrue(!CurrentWeapon);

	TArray<FComboAnimInfo*>* ComboInfos;
	float ComboCount;
	float ImpulseAmount;

	switch (CurrentWeapon->GetComboType())
	{
	case EComboType::E_ComboType_1:
		ComboInfos = MComboType1Structs.Find(CurrentWeapon->GetWeaponType());
		ComboCount = CurrentWeapon->GetComboCount();
		ImpulseAmount = (*ComboInfos)[ComboCount]->ImpulseAmount;
		break;
	case EComboType::E_ComboType_2 :
		ComboInfos = MComboType2Structs.Find(CurrentWeapon->GetWeaponType());
		ComboCount = CurrentWeapon->GetComboCount();
		ImpulseAmount = (*ComboInfos)[ComboCount]->ImpulseAmount;
		break;
	}

	// #0. Impulse Amount 설정
	CurrentWeapon->SetSkillNum(-1);
	CurrentWeapon->SetIsSkillAttack(false);
	CurrentWeapon->SetImpulseAmount(ImpulseAmount);
	CurrentWeapon->SetSkillBasePower(0.f);
	// #1. Attack 호출
	CurrentWeapon->Attack();

}

void UC_WeaponComponent::EndAttack()
{

	if(!!CurrentWeapon)
		CurrentWeapon->EndAttack();

}

void UC_WeaponComponent::AttackSkill(uint8 InSkillNum)
{

	CheckTrue( !OwnerObject
		|| OwnerObject->GetOwnerStateType() != EStateType::E_Idle
		|| !OwnerObject->GetManaComponent()->GetAttackSillValid(InSkillNum)
		|| !CurrentWeapon);

	TArray<FSkillAnimInfo*>* SkillInfos;
	float ImpulseAmount;
	float SkillBasePower;

	// #0. Skill Infos
	SkillInfos = MAttackSkillStructs.Find(CurrentWeapon->GetWeaponType());
	CheckTrue((*SkillInfos)[InSkillNum]->ManaRequired > OwnerObject->GetManaComponent()->GetLeftMana());

	ImpulseAmount = (*SkillInfos)[InSkillNum]->ImpulseAmount;
	SkillBasePower = (*SkillInfos)[InSkillNum]->BaseDamage;

	// #1. Impulse Amount 설정
	CurrentWeapon->SetSkillNum(InSkillNum);
	CurrentWeapon->SetIsSkillAttack(true);
	CurrentWeapon->SetImpulseAmount(ImpulseAmount);
	CurrentWeapon->SetSkillBasePower(SkillBasePower);
	// #2. AttackSkill 호출
	CurrentWeapon->AttackSkill(InSkillNum);
	// #3. Mana
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	OnRequireMana.ExecuteIfBound(InSkillNum, (*SkillInfos)[InSkillNum]->ManaRequired, (*SkillInfos)[InSkillNum]->CoolTimeRequired);
	// #4. Effect
	OnActivateFX.ExecuteIfBound(CurrentWeapon->GetWeaponType());

}

void UC_WeaponComponent::EndAttackSkill()
{

	CheckTrue(!OwnerObject || !CurrentWeapon);

	CurrentWeapon->EndAttackSkill();

}

void UC_WeaponComponent::GuardSkill()
{

	CheckTrue(!OwnerObject 
		|| OwnerObject->GetOwnerStateType() != EStateType::E_Idle
		|| OwnerObject->GetCharacterMovement()->IsFalling()
		|| !CurrentWeapon
	);

	CurrentWeapon->GuardSkill();

}

void UC_WeaponComponent::GuardSkill_Block()
{

	CheckTrue(!OwnerObject || !CurrentWeapon);

	CurrentWeapon->GuardSkill_Block();

}

void UC_WeaponComponent::EndGuardSkill()
{

	CheckTrue(!CurrentWeapon);

	CurrentWeapon->EndGuardSkill();

}
