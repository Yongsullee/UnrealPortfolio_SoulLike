#include "ComboWeapon.h"

#include "Animation/AnimMontage.h"
#include "Particles/ParticleSystem.h"

#include "Interface/OwnerWeaponInterface.h"

#include "GameFramework/Character.h"

AComboWeapon::AComboWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	ComboType = EComboType::E_ComboType_1;
}

void AComboWeapon::BeginPlay()
{

	Super::BeginPlay();

}

void AComboWeapon::SetEnableNextCombo(bool b)
{
	bEnableNextCombo = b;
}

void AComboWeapon::PlayNextCombo()
{

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	IOwnerWeaponInterface* Interface = Cast<IOwnerWeaponInterface>(OwnerCharacter);

	CheckTrue(!OwnerCharacter || !Interface);

	if (bExistNextCombo == true)
	{
		// #1. Anim Montage -> Montage Component
		OnPlayAnimMontage.ExecuteIfBound(EStateType::E_Attacking, WeaponType, ComboType, ComboCount, SkillNum);

		switch (Interface->GetOwnerComboType())
		{
		case EComboType::E_ComboType_1:
			if (2 == ComboCount)
				bLastCombo = true;
			break;
		case EComboType::E_ComboType_2:
			if (2 == ComboCount)
				bLastCombo = true;
			break;
		}

		bExistNextCombo = false;
	}

}

void AComboWeapon::Attack()
{

	if (bEnableNextCombo == true)
	{
		bExistNextCombo = true;
		ComboCount++;

		bEnableNextCombo = false;
	}
	else
	{
		if (IsOwnerCanAttack(this))
		{
			Super::Attack();

			// #1. Anim Montage -> Montage Component
			OnPlayAnimMontage.ExecuteIfBound(EStateType::E_Attacking, WeaponType, ComboType, ComboCount, SkillNum);
		}
	}

}

void AComboWeapon::EndAttack()
{
	
	Super::EndAttack();

	bExistNextCombo = false;
	bLastCombo = false;
	ComboCount = 0;
	ImpulseAmount = 0.f;

}
