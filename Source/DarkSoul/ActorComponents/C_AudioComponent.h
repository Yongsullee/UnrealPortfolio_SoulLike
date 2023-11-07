#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Global/Custom_Enums.h"

#include "C_AudioComponent.generated.h"

DECLARE_DYNAMIC_DELEGATE_FourParams(FSendEffectSound, class USoundCue*, InSoundCue1, class USoundCue*, InSoundCue2,
	const class AActor*, InActor1, const class AActor*, InActor2);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKSOUL_API UC_AudioComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UC_AudioComponent();

protected:
	virtual void BeginPlay() override;

private:
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


private:
	void LoadCommonHumanSoundFromDataTable();
	void LoadComboTypeSoundsFromDataTable();
	void LoadAttackSkillSoundsFromDataTable();

private:
	UFUNCTION()
		void SendSound(const class AActor* InActor, const EStateType InStateType, const EWeaponType InWeaponType, const EComboType InComboType = EComboType::E_Max, const uint8 InComboCount = -1, const uint8 InSkillNum = -1);

private:
	void SendHitSound(const class AActor* InActor, const EWeaponType& InWeaponType);
	void SendKnockDownSound(const class AActor* InActor, const EWeaponType& InWeaponType);
	void SendDeathSound(const class AActor* InActor, const EWeaponType& InWeaponType);
	void SendComboTypeSound(const class AActor* InActor, const EWeaponType& InWeaponType, const EComboType& InComboType, uint8 InComboCount);
	void SendAttackSkillSound(const class AActor* InActor, const EWeaponType& InWeaponType, uint8 InSkillNum);

public:
	FSendEffectSound OnSendEffectSound;

private:
	class AGameObject* OwnerObject;
	class UDA_Weapon* DataAsset;

private:
	class UC_WeaponComponent* WeaponComp;

private:
	TMultiMap<EWeaponType, class USoundCue*> MmHitHumanSounds;
	TMultiMap<EWeaponType, class USoundCue*> MmKnockDownHumanSounds;
	TMultiMap<EWeaponType, class USoundCue*> MmDeathHumanSounds;

	TMap<EWeaponType, TArray<class USoundCue*>> MComboType1HumanSounds;
	TMap<EWeaponType, TArray<class USoundCue*>> MComboType2HumanSounds;
	TMap<EWeaponType, TArray<class USoundCue*>> MSkillHumanSounds;

	TMap<EWeaponType, TArray<class USoundCue*>> MComboType1WeaponSounds;
	TMap<EWeaponType, TArray<class USoundCue*>> MComboType2WeaponSounds;
	TMap<EWeaponType, TArray<class USoundCue*>> MSkillWeaponSounds;

};
