#include "C_FXComponent.h"
#include "Global/Global.h"

#include "DataAssets/DA_Weapon.h"

#include "GameObjects/GameObject.h"
#include "ActorComponents/C_WeaponComponent.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

UC_FXComponent::UC_FXComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UC_FXComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerObject = Cast<AGameObject>(GetOwner());
	DataAsset = OwnerObject->GetWeaponDataAsset();
	CheckTrue(!OwnerObject || !DataAsset);

	// #0. Attack Skil Info
	LoadAttackSkillInfoFromDataTable();

	UC_WeaponComponent* WeaponComp = OwnerObject->FindComponentByClass<UC_WeaponComponent>();
	CheckTrue(!WeaponComp);

	WeaponComp->OnActivateFX.BindUFunction(this, "StartAttachFXActivation");

}

void UC_FXComponent::LoadAttackSkillInfoFromDataTable()
{

	if (!!OwnerObject && !!DataAsset)
	{
		// #0. Weapon Types
		TArray<FString> WeaponTypeAsStrings;
		CHelpers::GetEWeaponTypeAsStringArray(WeaponTypeAsStrings);

		// #1. DT & Structs
		TArray<UDataTable*> AttackSkillDataTables = DataAsset->GetAttackSkillInfos();
		TArray<FSkillAnimInfo*> SkillInfo_Structs;

		// #2. FXInfo
		FFxInfo FXStruct;
		FXStruct.FX = nullptr;
		FXStruct.PlayRate = 0.f;

		for (auto DataTable : AttackSkillDataTables)
		{
			TArray<FName>RowNames = DataTable->GetRowNames();
			FString RowNameStr = (RowNames[0].ToString()).LeftChop(2);
			const FString ContextString(TEXT("Attack Skill Info"));

			for (uint8 i = 0; i < WeaponTypeAsStrings.Num(); i++)
			{
				if (WeaponTypeAsStrings[i].Equals(RowNameStr))
				{
					DataTable->GetAllRows<FSkillAnimInfo>(ContextString, SkillInfo_Structs);
					if (!!(SkillInfo_Structs[0]->GhostEffect) && !!(SkillInfo_Structs[0]->AnimMontage))
					{
						FXStruct.FX = SkillInfo_Structs[0]->GhostEffect;
						FXStruct.PlayRate = SkillInfo_Structs[0]->AnimMontage->GetPlayLength();
						MGhostEffects.Add(static_cast<EWeaponType>(i), FXStruct);
						SkillInfo_Structs.Empty();
					}
					break;
				}
			}
		}
	}

}

void UC_FXComponent::StartAttachFXActivation(EWeaponType InEnum)
{

	if (MGhostEffects.Contains(InEnum))
	{
		FFxInfo* GhostFXInfo = MGhostEffects.Find(InEnum);
		if (GhostFXInfo->FX)
		{
			UNiagaraComponent* AttachEffect = UNiagaraFunctionLibrary::SpawnSystemAttached(
				GhostFXInfo->FX,
				OwnerObject->GetMesh(),
				"Attach FX Activation",
				FVector::ZeroVector,
				FRotator::ZeroRotator,
				EAttachLocation::KeepRelativeOffset,
				false
			);

			FTimerHandle DeactivateHandler;
			GetWorld()->GetTimerManager().SetTimer(DeactivateHandler, FTimerDelegate::CreateLambda([&, AttachEffect]() {
				AttachEffect->Deactivate();
				}), GhostFXInfo->PlayRate-0.3f, false);

		}
	}
	else
	{
		PrintLine();
		return;
	}

}
