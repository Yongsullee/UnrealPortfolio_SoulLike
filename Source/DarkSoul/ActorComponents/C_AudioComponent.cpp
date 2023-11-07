#include "C_AudioComponent.h"
#include "Global/Custom_Structs.h"

#include "GameObjects/GameObject.h"
#include "DataAssets/DA_Weapon.h"

#include "ActorComponents/C_WeaponComponent.h"

#include "Sound/SoundCue.h"

UC_AudioComponent::UC_AudioComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UC_AudioComponent::BeginPlay()
{

	Super::BeginPlay();
	
	OwnerObject = Cast<AGameObject>(GetOwner());
	DataAsset = OwnerObject->GetWeaponDataAsset();

	CheckTrue(!OwnerObject || !DataAsset );

	// #1. Load DT	
	LoadCommonHumanSoundFromDataTable();
	LoadComboTypeSoundsFromDataTable();
	LoadAttackSkillSoundsFromDataTable();

	// #2. Delegates
	OwnerObject->OnPlayHumanSound.BindUFunction(this, "SendSound");

}

void UC_AudioComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

	WeaponComp = OwnerObject->FindComponentByClass<UC_WeaponComponent>();

	TArray<AWeapon*> Weapons;
	Weapons = WeaponComp->GetWeapons();
	for (const auto& Weapon : Weapons)
	{
		if (Weapon)
		{
			Weapon->OnPlayWeaponSound.BindUFunction(this, "SendSound");
		}
	}

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UC_AudioComponent::LoadCommonHumanSoundFromDataTable()
{

	if (!!OwnerObject && !!DataAsset)
	{
		UDataTable* HitAnimInfo = DataAsset->GetHitInfo();
		UDataTable* KnockDownAnimInfo = DataAsset->GetKnockDownInfo();
		UDataTable* DeathAnimInfo = DataAsset->GetDeathInfo();

		CHelpers::LoadCommonHumanSoundFromDataTable(&HitAnimInfo, MmHitHumanSounds);
		CHelpers::LoadCommonHumanSoundFromDataTable(&KnockDownAnimInfo, MmKnockDownHumanSounds);
		CHelpers::LoadCommonHumanSoundFromDataTable(&DeathAnimInfo, MmDeathHumanSounds);
	}

}

void UC_AudioComponent::LoadComboTypeSoundsFromDataTable()
{

	if (!!OwnerObject && !!DataAsset)
	{
		TArray<UDataTable*> ComboType1Infos = DataAsset->GetComboType1_Infos();
		TArray<UDataTable*> ComboType2Infos = DataAsset->GetComboType2_Infos();

		// #1. Human Sound
		CHelpers::LoadComboTypeHumanSoundFromDataTable(&ComboType1Infos, MComboType1HumanSounds);
		CHelpers::LoadComboTypeWeaponSoundFromDataTable(&ComboType1Infos, MComboType1WeaponSounds);
		// #2. Weapon Sound
		CHelpers::LoadComboTypeHumanSoundFromDataTable(&ComboType2Infos, MComboType2HumanSounds);
		CHelpers::LoadComboTypeWeaponSoundFromDataTable(&ComboType2Infos, MComboType2WeaponSounds);
	}
	else
	{
		PrintLine();
		return;
	}

}

void UC_AudioComponent::LoadAttackSkillSoundsFromDataTable()
{

	if (!!OwnerObject && !!DataAsset)
	{
		TArray<UDataTable*> AttackSkillInfos = DataAsset->GetAttackSkillInfos();
		// #1. Human Sound
		CHelpers::LoadSkillHumanSoundFromFromDataTable(&AttackSkillInfos, MSkillHumanSounds);
		// #2. Weapon Sound
		CHelpers::LoadSkillWeaponSoundFromFromDataTable(&AttackSkillInfos, MSkillWeaponSounds);
	}
	else
	{
		PrintLine();
		return;
	}

}

// [23-06-30] Sound Event 
void UC_AudioComponent::SendSound(const class AActor* InActor, const EStateType InStateType, const EWeaponType InWeaponType, const EComboType InComboType, const uint8 InComboCount, const uint8 InSkillNum)
{

	CheckTrue(!InActor);

	switch (InStateType)
	{
	case EStateType::E_Stumbling :
		SendHitSound(InActor, InWeaponType);
		break;
	case EStateType::E_KnockDown :
		SendKnockDownSound(InActor, InWeaponType);
		break;
	case EStateType::E_Dead : 
		SendDeathSound(InActor, InWeaponType);
		break;
	case EStateType::E_Attacking:
		SendComboTypeSound(InActor, InWeaponType, InComboType, InComboCount);
		break;
	case EStateType::E_Skilling :
		PrintLine();
		SendAttackSkillSound(InActor, InWeaponType, InSkillNum);
		break;
	}
	
}

void UC_AudioComponent::SendHitSound(const AActor* InActor, const EWeaponType& InWeaponType)
{
	TArray<USoundCue*> HitSoundCues;

	if (MmHitHumanSounds.Num(InWeaponType) == 0)
	{
		return;
	}

	MmHitHumanSounds.MultiFind(InWeaponType, HitSoundCues);

	if (HitSoundCues.Num() == 0)
	{
		return;
	}

	uint8 Idx = FMath::RandRange(0, HitSoundCues.Num() - 1); // Use the actual size of the array

	if (!HitSoundCues.IsValidIndex(Idx))
	{
		return;
	}

	USoundCue* HitSoundCue = HitSoundCues[Idx];

	if (HitSoundCue)
	{
		OnSendEffectSound.ExecuteIfBound(
			HitSoundCue,
			nullptr,
			OwnerObject,
			nullptr
		);
	}

}

void UC_AudioComponent::SendKnockDownSound(const AActor* InActor, const EWeaponType& InWeaponType)
{

	if (MmKnockDownHumanSounds.Num(InWeaponType) > 1)
	{
		TArray<USoundCue*> KnockDownSoundCues;
		MmKnockDownHumanSounds.MultiFind(InWeaponType, KnockDownSoundCues);

		USoundCue* KnockDownSoundCue = KnockDownSoundCues[FMath::RandRange(0, MmHitHumanSounds.Num(InWeaponType) - 1)];
		if (!!KnockDownSoundCue)
		{
			OnSendEffectSound.ExecuteIfBound(
				KnockDownSoundCue,
				nullptr,
				OwnerObject,
				nullptr
			);
		}
	}
	else
	{
		OnSendEffectSound.ExecuteIfBound(
			(*MmKnockDownHumanSounds.Find(InWeaponType)),
			nullptr,
			OwnerObject,
			nullptr
		);
	}

}

void UC_AudioComponent::SendDeathSound(const AActor* InActor, const EWeaponType& InWeaponType)
{

	if (MmDeathHumanSounds.Contains(InWeaponType))
	{
		if (MmDeathHumanSounds.Num(InWeaponType) > 1)
		{	
			TArray<USoundCue*> DeathSoundCues;
			MmDeathHumanSounds.MultiFind(InWeaponType, DeathSoundCues);

			int8 Index = FMath::RandRange(0, MmHitHumanSounds.Num(InWeaponType) - 1);

			if (DeathSoundCues.IsValidIndex(Index))
			{
				USoundCue* DeathSoundCue = DeathSoundCues[Index];
				if (DeathSoundCue)
				{
					OnSendEffectSound.ExecuteIfBound(
						DeathSoundCue,
						nullptr,
						OwnerObject,
						nullptr
					);
				}
			}
		}
		else
		{
			USoundCue** DeathSoundCue = MmDeathHumanSounds.Find(InWeaponType);

			if ((*DeathSoundCue))
			{
				OnSendEffectSound.ExecuteIfBound(
					(*DeathSoundCue),
					nullptr,
					OwnerObject,
					nullptr
				);
			}
		}
	}

}

// [2023-06-30] Sound Event Queue
void UC_AudioComponent::SendComboTypeSound(const AActor* InActor, const EWeaponType& InWeaponType, const EComboType& InComboType, uint8 InComboCount)
{

	CheckTrue(!OwnerObject);

	TMap<EWeaponType, TArray<USoundCue*>>* HumanSoundCueMap = nullptr;
	TMap<EWeaponType, TArray<USoundCue*>>* WeaponSoundCueMap = nullptr;

	switch (InComboType)
	{
	case EComboType::E_NoCombo:
		HumanSoundCueMap = &MComboType1HumanSounds;
		WeaponSoundCueMap = &MComboType1WeaponSounds;
		break;
	case EComboType::E_ComboType_1:
		HumanSoundCueMap = &MComboType1HumanSounds;
		WeaponSoundCueMap = &MComboType1WeaponSounds;
		break;
	case EComboType::E_ComboType_2:
		HumanSoundCueMap = &MComboType2HumanSounds;
		WeaponSoundCueMap = &MComboType2WeaponSounds;
		break;
	case EComboType::E_Max:
	default:
		PrintLine();
		return; // No valid combo type provided.
	}

	if (HumanSoundCueMap && HumanSoundCueMap->Contains(InWeaponType)
		&& WeaponSoundCueMap && WeaponSoundCueMap->Contains(InWeaponType))
	{
		TArray<USoundCue*>* HumanSoundCues = HumanSoundCueMap->Find(InWeaponType);
		TArray<USoundCue*>* WeaponSoundCues = WeaponSoundCueMap->Find(InWeaponType);

		if ((*HumanSoundCues).Num() > 0 && (*WeaponSoundCues).Num() > 0)
		{
			uint8 HumanSoundCuesIndex = InComboType == EComboType::E_NoCombo ? FMath::RandRange(0, (*HumanSoundCues).Num() - 1) : InComboCount;
			uint8 WeaponSoundCuesIndex = InComboType == EComboType::E_NoCombo ? FMath::RandRange(0, (*WeaponSoundCues).Num() - 1) : InComboCount;

			if ((*HumanSoundCues).IsValidIndex(HumanSoundCuesIndex) && (*WeaponSoundCues).IsValidIndex(WeaponSoundCuesIndex))
			{
				USoundCue* HumanSoundCue = (*HumanSoundCues)[HumanSoundCuesIndex];
				USoundCue* WeaponSoundCue = (*WeaponSoundCues)[WeaponSoundCuesIndex];

				if(HumanSoundCue && WeaponSoundCue)
				{
					OnSendEffectSound.ExecuteIfBound(
						HumanSoundCue,
						WeaponSoundCue,
						OwnerObject,
						InActor
					);
				}
			}
		}
	}

}

// [2023-06-30] Sound Event Queue
void UC_AudioComponent::SendAttackSkillSound(const AActor* InActor, const EWeaponType& InWeaponType, uint8 InSkillNum)
{

	if (!MSkillHumanSounds.Contains(InWeaponType)
		&& !MSkillWeaponSounds.Contains(InWeaponType))
	{
		PrintLine();
		return;
	}
	
	TArray<USoundCue*>* HumanSoundCues = nullptr;
	USoundCue* HumanSoundCue = nullptr;

	TArray<USoundCue*>* WeaponSoundCues = nullptr;
	USoundCue* WeaponSoundCue = nullptr;

	if (MSkillHumanSounds.Contains(InWeaponType))
	{
		HumanSoundCues = MSkillHumanSounds.Find(InWeaponType);

		if (HumanSoundCues->IsValidIndex(InSkillNum))
			HumanSoundCue = (*HumanSoundCues)[InSkillNum];
	}

	if (MSkillWeaponSounds.Contains(InWeaponType))
	{
		WeaponSoundCues = MSkillWeaponSounds.Find(InWeaponType);

		if(WeaponSoundCues->IsValidIndex(InSkillNum))
			WeaponSoundCue = (*WeaponSoundCues)[InSkillNum];
	}

	if (HumanSoundCue || WeaponSoundCue)
	{
		OnSendEffectSound.ExecuteIfBound(
			HumanSoundCue ? HumanSoundCue : nullptr,
			WeaponSoundCue ? WeaponSoundCue : nullptr,
			OwnerObject,
			InActor
		);
	}
	
}