#pragma once

#include "CoreMinimal.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"

#include "Global/Custom_Enums.h"
#include "Global/Custom_Structs.h"

#include "Sound/SoundCue.h"

#include "DataAssets/DA_Weapon.h"

class DARKSOUL_API CHelpers
{
public:
	template<typename T>
	static void GetAsset(T** OutObject, FString InPath)
	{
		ConstructorHelpers::FObjectFinder<T> asset(*InPath);
		verifyf(asset.Succeeded(), L"asset.Succeeded()");

		*OutObject = asset.Object;
	}

	template<typename T>
	static void GetAssetDynamic(T** OutObject, FString InPath)
	{
		T* obj = Cast<T>(StaticLoadObject(T::StaticClass(), NULL, *InPath));
		verifyf(!!obj, L"!!asset");

		*OutObject = obj;
	}

	template<typename T>
	static void GetClass(TSubclassOf<T>* OutClass, FString InPath)
	{
		ConstructorHelpers::FClassFinder<T> asset(*InPath);
		verifyf(asset.Succeeded(), L"asset.Succeeded()");

		*OutClass = asset.Class;
	}

	template<typename T>
	static void CreateComponent(AActor* InActor, T** InComponent, FName InName, USceneComponent* InParent = NULL)
	{
		*InComponent = InActor->CreateDefaultSubobject<T>(InName);

		if (!!InParent)
		{
			(*InComponent)->SetupAttachment(InParent);

			return;
		}

		InActor->SetRootComponent((*InComponent));
	}

	template<typename T>
	static T* GetComponent(AActor* InActor)
	{
		return Cast<T>(InActor->GetComponentByClass(T::StaticClass()));
	}

	// Remark [CHelpers, Static]
	static FString GetEStateTypeAsString(EStateType EnumValue)
	{
		const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EStateType"), true);
		if (!enumPtr)
		{
			return FString("Invalid");
		}
		return enumPtr->GetNameStringByIndex((int32)EnumValue).Right(enumPtr->GetNameStringByIndex((int32)EnumValue).Len() - 2);
	}

	// Remark [CHelpers, Static]
	static void GetEStateTypeAsStringArray(TArray<FString>& OutArr)
	{
		// CombatType
		for (int32 i = 0; i < static_cast<int>(EStateType::E_Max); i++)
		{
			EStateType CombatType = static_cast<EStateType>(i);
			FString CombatTypeAsString = GetEStateTypeAsString(CombatType);
			OutArr.Add(CombatTypeAsString);
		}
	}

	// Remark [CHelpers, Static]
	static FString GetEPlayerCombatTypeAsString(EPlayerCombatType EnumValue)
	{
		const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EPlayerCombatType"), true);
		if (!enumPtr)
		{
			return FString("Invalid");
		}
		return enumPtr->GetNameStringByIndex((int32)EnumValue).Right(enumPtr->GetNameStringByIndex((int32)EnumValue).Len() - 2);
	}

	// Remark [CHelpers, Static]
	static void GetEPlayerCombatTypeAsStringArray(TArray<FString>& OutArr)
	{
		// CombatType
		for (int32 i = 0; i < static_cast<int>(EPlayerCombatType::E_Max); i++)
		{
			EPlayerCombatType CombatType = static_cast<EPlayerCombatType>(i);
			FString CombatTypeAsString = GetEPlayerCombatTypeAsString(CombatType);
			OutArr.Add(CombatTypeAsString);
		}
	}

	// Remark [CHelpers, Static]
	static FString GetEWeaponTypeAsString(EWeaponType EnumValue)
	{
		const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EWeaponType"), true);
		if (!enumPtr)
		{
			return FString("Invalid");
		}
		return enumPtr->GetNameStringByIndex((int32)EnumValue).Right(enumPtr->GetNameStringByIndex((int32)EnumValue).Len() - 2);
	}

	// Remark [CHelpers, Static]
	static void GetEWeaponTypeAsStringArray(TArray<FString>& OutArr)
	{
		// CombatType
		for (int32 i = 0; i < static_cast<int>(EWeaponType::E_Max); i++)
		{
			EWeaponType WeaponType = static_cast<EWeaponType>(i);
			FString WeaponTypeAsString = GetEWeaponTypeAsString(WeaponType);
			OutArr.Add(WeaponTypeAsString);
		}
	}

	// Remark [CHelpers, Static]
	static FString GetEMagicProjectileTypeAsString(EProjectileType EnumValue)
	{
		const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EProjectileType"), true);
		if (!enumPtr)
		{
			return FString("Invalid");
		}
		return enumPtr->GetNameStringByIndex((int32)EnumValue).Right(enumPtr->GetNameStringByIndex((int32)EnumValue).Len() - 2);
	}

	// Remark [CHelpers, Static]
	static void GetEMagicProjectileTypeAsStringArray(TArray<FString>& OutArr)
	{
		// CombatType
		for (int32 i = 0; i < static_cast<int>(EProjectileType::E_Max); i++)
		{
			EProjectileType ProjectileType = static_cast<EProjectileType>(i);
			FString ProjectileTypeAsString = GetEMagicProjectileTypeAsString(ProjectileType);
			OutArr.Add(ProjectileTypeAsString);
		}
	}

	// [23-04-27] : Load 함수
	static void LoadSpecInfoFromDataTable(UDataTable** InDataTable, FSpecInfo& OutStruct, FString InString)
	{

		if (!!(*InDataTable))
		{
			TArray<FName>RowNames = (*InDataTable)->GetRowNames();
			for (auto RowName : RowNames)
			{
				FString RowNameStr = RowName.ToString();
				const FString ContextString(TEXT("Owner Spec Info"));

				if (InString.Contains(RowNameStr))
				{
					FSpecInfo* RowStruct = (*InDataTable)->FindRow<FSpecInfo>(RowName, ContextString, true);
					OutStruct = *RowStruct;
				}
			}
		}

	}

	// [23-04-27] : Load 함수
	static void LoadPowerInfoFromDataTable(UDataTable** InDataTable, FPowerInfo& OutStruct, FString InString)
	{

		if (!!(*InDataTable))
		{
			TArray<FName>RowNames = (*InDataTable)->GetRowNames();
			const FString ContextString(TEXT("Weapon Power Info"));
			for (auto RowName : RowNames)
			{
				FString RowNameStr = RowName.ToString();

				if (InString.Contains(RowNameStr))
				{
					FPowerInfo* RowStruct = (*InDataTable)->FindRow<FPowerInfo>(RowName, ContextString, true);
					OutStruct = *RowStruct;
				}
			}
		}

	}

	// [23-04-27] : Load 함수
	static void LoadCommonAnimInfoFromDataTable(UDataTable** InDataTable, TMultiMap<EWeaponType, FCommonAnimInfo>& InMMap)
	{
		if (!!(*InDataTable))
		{
			TArray<FName>RowNames = (*InDataTable)->GetRowNames();
			
			
			TArray<FString> WeaponTypeAsStrings;
			CHelpers::GetEWeaponTypeAsStringArray(WeaponTypeAsStrings);

			const FString ContextString(TEXT("Common Anim Info"));

			for (auto RowName : RowNames)
			{
				FString RowNameStr = (RowName.ToString()).LeftChop(2);

				for (int32 i = 0; i < WeaponTypeAsStrings.Num(); i++)
				{
					if (RowNameStr.Equals(WeaponTypeAsStrings[i]))
					{
						FCommonAnimInfo CommonAnimInfo_Struct;
						FCommonAnimInfo* RowStruct = (*InDataTable)->FindRow<FCommonAnimInfo>(RowName, ContextString, true);
						CommonAnimInfo_Struct = *RowStruct;

						InMMap.Add(static_cast<EWeaponType>(i), CommonAnimInfo_Struct);
						break;
					}
				}
			}
		}

	}

	// [23-04-27] : Load 함수
	static void LoadCommonHumanSoundFromDataTable(UDataTable** InDataTable, TMultiMap<EWeaponType, USoundCue*>& OutMap)
	{

		if (!!(*InDataTable))
		{
			TArray<FName>RowNames = (*InDataTable)->GetRowNames();


			TArray<FString> WeaponTypeAsStrings;
			CHelpers::GetEWeaponTypeAsStringArray(WeaponTypeAsStrings);

			const FString ContextString(TEXT("Common Anim Info"));

			for (auto RowName : RowNames)
			{
				FString RowNameStr = (RowName.ToString()).LeftChop(2);

				for (int32 i = 0; i < WeaponTypeAsStrings.Num(); i++)
				{
					if (RowNameStr.Equals(WeaponTypeAsStrings[i]))
					{
						FCommonAnimInfo* RowStruct = (*InDataTable)->FindRow<FCommonAnimInfo>(RowName, ContextString, true);
						OutMap.Add(static_cast<EWeaponType>(i), (*RowStruct).HumanSound);
						break;
					}
				}
			}
		}

	}

	// [23-04-27] : Load 함수
	static void LoadComboTypeInfoFromDataTable(TArray<UDataTable*>* InDataTables, TMap<EWeaponType, TArray<FComboAnimInfo*>>& OutMap)
	{

		TArray<FString> WeaponTypeAsStrings;
		CHelpers::GetEWeaponTypeAsStringArray(WeaponTypeAsStrings);

		if (!!(*InDataTables).IsValidIndex(0))
		{
			for (int i = 0; i < (*InDataTables).Num(); i++)
			{
				TArray<FName>RowNames = (*InDataTables)[i]->GetRowNames();
				FString RowNameStr = (RowNames[0].ToString()).LeftChop(2);
				const FString ContextString(TEXT("Combo Info"));

				for (uint8 j = 0; j < WeaponTypeAsStrings.Num(); j++)
				{
					if (WeaponTypeAsStrings[j].Equals(RowNameStr))
					{
						TArray<FComboAnimInfo*> ComboAnimInfo_Structs;
						(*InDataTables)[i]->GetAllRows<FComboAnimInfo>(ContextString, ComboAnimInfo_Structs);
						OutMap.Add(static_cast<EWeaponType>(j), ComboAnimInfo_Structs);
						ComboAnimInfo_Structs.Empty();
						break;
					}
				}
			}
		}
	}

	// [23-06-14] Weapon Sound
	static void LoadComboTypeWeaponSoundFromDataTable(TArray<UDataTable*>* InDataTables, TMap<EWeaponType, TArray<USoundCue*>>& OutMap)
	{
		TArray<FString> WeaponTypeAsStrings;
		CHelpers::GetEWeaponTypeAsStringArray(WeaponTypeAsStrings);

		if ((*InDataTables).IsValidIndex(0))
		{
			for (int i = 0; i < (*InDataTables).Num(); i++)
			{
				TArray<FName> RowNames = (*InDataTables)[i]->GetRowNames();
				FString RowNameStr = (RowNames[0].ToString()).LeftChop(2);
				const FString ContextString(TEXT("Combo Info"));

				for (uint8 j = 0; j < WeaponTypeAsStrings.Num(); j++)
				{
					if (WeaponTypeAsStrings[j].Equals(RowNameStr))
					{
						TArray<FComboAnimInfo*> ComboAnimInfo_Structs;
						(*InDataTables)[i]->GetAllRows<FComboAnimInfo>(ContextString, ComboAnimInfo_Structs);

						TArray<USoundCue*> SoundCues;
						for (const FComboAnimInfo* ComboInfo : ComboAnimInfo_Structs)
						{
							if(!!ComboInfo->WeaponSound)
								SoundCues.Add(ComboInfo->WeaponSound);
						}

						OutMap.Add(static_cast<EWeaponType>(j), SoundCues);
						break;
					}
				}
			}
		}
	}

	// [23-06-14] Impact Sound
	static void LoadComboTypeHumanSoundFromDataTable(TArray<UDataTable*>* InDataTables, TMap<EWeaponType, TArray<USoundCue*>>& OutMap)
	{
		TArray<FString> WeaponTypeAsStrings;
		CHelpers::GetEWeaponTypeAsStringArray(WeaponTypeAsStrings);

		if ((*InDataTables).IsValidIndex(0))
		{
			for (int i = 0; i < (*InDataTables).Num(); i++)
			{
				TArray<FName> RowNames = (*InDataTables)[i]->GetRowNames();
				FString RowNameStr = (RowNames[0].ToString()).LeftChop(2);
				const FString ContextString(TEXT("Combo Info"));

				for (uint8 j = 0; j < WeaponTypeAsStrings.Num(); j++)
				{
					if (WeaponTypeAsStrings[j].Equals(RowNameStr))
					{
						TArray<FComboAnimInfo*> ComboAnimInfo_Structs;
						(*InDataTables)[i]->GetAllRows<FComboAnimInfo>(ContextString, ComboAnimInfo_Structs);

						TArray<USoundCue*> SoundCues;
						for (const FComboAnimInfo* ComboInfo : ComboAnimInfo_Structs)
						{
							if (!!ComboInfo->HumanSound)
								SoundCues.Add(ComboInfo->HumanSound);
						}

						OutMap.Add(static_cast<EWeaponType>(j), SoundCues);
						break;
					}
				}
			}
		}
	}

	// [23-04-27] : Load 함수
	static void LoadSkillInfoFromDataTable(TArray<UDataTable*>* InDataTables, TMap<EWeaponType, TArray<FSkillAnimInfo*>>& OutMap)
	{

		TArray<FString> WeaponTypeAsStrings;
		CHelpers::GetEWeaponTypeAsStringArray(WeaponTypeAsStrings);

		if (!!(*InDataTables).IsValidIndex(0))
		{
			for (int i = 0; i < (*InDataTables).Num(); i++)
			{
				TArray<FName>RowNames = (*InDataTables)[i]->GetRowNames();
				FString RowNameStr = (RowNames[0].ToString()).LeftChop(2);
				const FString ContextString(TEXT("Attack Skill Info"));

				for (uint8 j = 0; j < WeaponTypeAsStrings.Num(); j++)
				{
					if (WeaponTypeAsStrings[j].Equals(RowNameStr))
					{
						TArray<FSkillAnimInfo*> SkillInfo_Structs;
						(*InDataTables)[i]->GetAllRows<FSkillAnimInfo>(ContextString, SkillInfo_Structs);
						OutMap.Add(static_cast<EWeaponType>(j), SkillInfo_Structs);
						SkillInfo_Structs.Empty();
						break;
					}
				}
			}
		}
	}

	// [23-06-14] Impact Sound
	static void LoadSkillWeaponSoundFromFromDataTable(TArray<UDataTable*>* InDataTables, TMap<EWeaponType, TArray<USoundCue*>>& OutMap)
	{

		TArray<FString> WeaponTypeAsStrings;
		CHelpers::GetEWeaponTypeAsStringArray(WeaponTypeAsStrings);

		if (!!(*InDataTables).IsValidIndex(0))
		{
			for (int i = 0; i < (*InDataTables).Num(); i++)
			{
				TArray<FName>RowNames = (*InDataTables)[i]->GetRowNames();
				FString RowNameStr = (RowNames[0].ToString()).LeftChop(2);
				const FString ContextString(TEXT("Attack Skill Info"));

				for (uint8 j = 0; j < WeaponTypeAsStrings.Num(); j++)
				{
					if (WeaponTypeAsStrings[j].Equals(RowNameStr))
					{
						TArray<FSkillAnimInfo*> SkillInfo_Structs;
						(*InDataTables)[i]->GetAllRows<FSkillAnimInfo>(ContextString, SkillInfo_Structs);
						TArray<USoundCue*> SoundCues;
						for (const FSkillAnimInfo* SkillInfo : SkillInfo_Structs)
						{
							if(!!SkillInfo->WeaponSound)
								SoundCues.Add(SkillInfo->WeaponSound);
						}

						OutMap.Add(static_cast<EWeaponType>(j), SoundCues);
						break;
					}
				}
			}
		}

	}

	// [23-06-14] Impact Sound
	static void LoadSkillHumanSoundFromFromDataTable(TArray<UDataTable*>* InDataTables, TMap<EWeaponType, TArray<USoundCue*>>& OutMap)
	{

		TArray<FString> WeaponTypeAsStrings;
		CHelpers::GetEWeaponTypeAsStringArray(WeaponTypeAsStrings);

		if (!!(*InDataTables).IsValidIndex(0))
		{
			for (int i = 0; i < (*InDataTables).Num(); i++)
			{
				TArray<FName>RowNames = (*InDataTables)[i]->GetRowNames();
				FString RowNameStr = (RowNames[0].ToString()).LeftChop(2);
				const FString ContextString(TEXT("Attack Skill Info"));

				for (uint8 j = 0; j < WeaponTypeAsStrings.Num(); j++)
				{
					if (WeaponTypeAsStrings[j].Equals(RowNameStr))
					{
						TArray<FSkillAnimInfo*> SkillInfo_Structs;	
						(*InDataTables)[i]->GetAllRows<FSkillAnimInfo>(ContextString, SkillInfo_Structs);
						TArray<USoundCue*> SoundCues;
						for (const FSkillAnimInfo* SkillInfo : SkillInfo_Structs)
						{
							if (!!SkillInfo->HumanSound)
								SoundCues.Add(SkillInfo->HumanSound);
						}

						OutMap.Add(static_cast<EWeaponType>(j), SoundCues);
						break;
					}
				}
			}
		}

	}

};