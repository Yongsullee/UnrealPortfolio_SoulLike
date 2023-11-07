#include "Weapon.h"
#include "Global/Global.h"

#include "GameFramework/Character.h"
#include "Interface/OwnerWeaponInterface.h"

#include "ActorComponents/C_WeaponComponent.h"
#include "ActorComponents/C_PowerComponent.h"
#include "ActorComponents/C_AudioComponent.h"

#include "NiagaraComponent.h"

AWeapon::AWeapon()
{

	PrimaryActorTick.bCanEverTick = false;

	PowerComponent = CreateDefaultSubobject<UC_PowerComponent>(TEXT("Power Component"));

	ItemInfoStruct.ItemType = EItemType::E_Weapon;

}

void AWeapon::BeginPlay()
{

	Super::BeginPlay();

}

void AWeapon::AttachComponentToOwnerComponent(USkeletalMeshComponent* InSkeletalMeshComponent)
{}

void AWeapon::OnSelected()
{
	SetActorHiddenInGame(false);
}

void AWeapon::OffSelected()
{
	SetActorHiddenInGame(true);
}


void AWeapon::OnCollision()
{}

void AWeapon::OffCollision()
{
	AttackHitActors.Empty();
}

void AWeapon::OnZoneEffect()
{

	CheckTrue(!ZoneEffect);

	ZoneEffect->Activate();

}

void AWeapon::OffZoneEffect()
{

	CheckTrue(!ZoneEffect);

	ZoneEffect->DeactivateImmediate();

}

// [23-06-22] Test
void AWeapon::PlaySound()
{

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	IOwnerWeaponInterface* Interface = Cast<IOwnerWeaponInterface>(OwnerCharacter);

	CheckTrue(!OwnerCharacter || !Interface);

	OnPlayWeaponSound.ExecuteIfBound(
		this, 
		Interface->GetOwnerStateType(), 
		WeaponType, 
		ComboType, 
		ComboCount, 
		SkillNum
	);

	PrintLine();

}


FInventoryItemInfo AWeapon::InteractWithInventory()
{
	return ItemInfoStruct;
}

float AWeapon::GetImpulseAmount()
{

	return ImpulseAmount;

}

/*	*************************************************************************

	[Remark	:	Interface를 통해 Owner Actor의 State Type 가져와 E_Idle여부 체크]

	목적:
		Owner Actor의 현재 무기가 본인이며, State Type이 E_Idle인지 확인합니다.
	설명:
		1. Interface->GetOwnerStateType() == EStateType::E_Idle
		2. Interface->GetCurrentWeapon() == this

*	*************************************************************************/

bool AWeapon::IsOwnerCanAttack(AWeapon* CurrentWeapon)
{

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	IOwnerWeaponInterface* Interface = Cast<IOwnerWeaponInterface>(OwnerCharacter);

	bool bCanAttack = true;

	bool IsOwnerIdle = false;
	bool IsCurrentWeapon = false;
	
	if (Interface == nullptr)
		return false;

	if (Interface->GetOwnerStateType() == EStateType::E_Idle)
		IsOwnerIdle = true;
	if (Interface->GetOwnerCurrentWeapon() == CurrentWeapon)
		IsCurrentWeapon = true;

	bCanAttack &= IsOwnerIdle;
	bCanAttack &= IsCurrentWeapon;

	return bCanAttack;

}

void AWeapon::Attack()
{

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	IOwnerWeaponInterface* Interface = Cast<IOwnerWeaponInterface>(OwnerCharacter);

	CheckTrue(!IsOwnerCanAttack(this) || !Interface);

	Interface->SetOwnerStateType(EStateType::E_Attacking);

}

void AWeapon::EndAttack()
{

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	IOwnerWeaponInterface* Interface = Cast<IOwnerWeaponInterface>(OwnerCharacter);

	CheckTrue(!OwnerCharacter || !Interface);

	Interface->SetOwnerStateType(EStateType::E_Idle);

}

/*	********************************************************************************************************
	
	[Remark]	:	Attack Skill 동작 수행 시 Mana Component에 바인딩 된 FAttackSkillStart 델리게이트 실행

	목적:
		Attack Skill 넘버 당 필요한 마나량 소모 + 쿨 타임 적용 동작을 수행하기 위함입니다.
	설명:
		1. FAttackSkillStart 커스텀 델리게이트 선언 및 호출
		2. Mana Component에서 바인딩한 FAttackSkillStart 커스텀 델리게이트를 수행도록 합니다.

*	********************************************************************************************************/

void AWeapon::AttackSkill(const int InSkillNum)
{

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	IOwnerWeaponInterface* Interface = Cast<IOwnerWeaponInterface>(OwnerCharacter);

	CheckTrue(!OwnerCharacter || !Interface);

	// #0. State
	Interface->SetOwnerStateType(EStateType::E_Skilling);

	// #1. Animation -> Montage Component
	OnPlayAnimMontage.ExecuteIfBound(Interface->GetOwnerStateType(), WeaponType, ComboType, ComboCount, InSkillNum);

	// #2. Buff
	OnBuffStatusEffect.Broadcast();

}

void AWeapon::EndAttackSkill()
{

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	IOwnerWeaponInterface* Interface = Cast<IOwnerWeaponInterface>(OwnerCharacter); 

	CheckTrue(!OwnerCharacter || !Interface);
	
	// #0. State
	Interface->SetOwnerStateType(EStateType::E_Idle);

}

void AWeapon::StartAttackDilation()
{

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.10f);
	FTimerHandle DilationHandler;
	GetWorld()->GetTimerManager().SetTimer(DilationHandler, FTimerDelegate::CreateLambda([&]() {
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);
		}), 0.04f, false);

}

// Guard Skill
void AWeapon::GuardSkill()
{

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	IOwnerWeaponInterface* Interface = Cast<IOwnerWeaponInterface>(OwnerCharacter);

	CheckTrue(!OwnerCharacter || !Interface);

	Interface->SetOwnerStateType(EStateType::E_Guarding);

	// #1. Anim Motnage -> Montage Component
	OnPlayAnimMontage.ExecuteIfBound(Interface->GetOwnerStateType(), WeaponType, ComboType, ComboCount, 0);

}

void AWeapon::GuardSkill_Block()
{

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	IOwnerWeaponInterface* Interface = Cast<IOwnerWeaponInterface>(OwnerCharacter);

	CheckTrue(!OwnerCharacter || !Interface);

	if (Interface->GetOwnerStateType() == EStateType::E_Guarding)
	{
		OnPlayAnimMontage.ExecuteIfBound(Interface->GetOwnerStateType(), WeaponType, ComboType, ComboCount, 1);
	}

}

void AWeapon::EndGuardSkill()
{

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	IOwnerWeaponInterface* Interface = Cast<IOwnerWeaponInterface>(OwnerCharacter);

	CheckTrue(!OwnerCharacter || !Interface);

	Interface->SetOwnerStateType(EStateType::E_Idle);

}