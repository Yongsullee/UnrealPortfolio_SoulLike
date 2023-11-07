#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Custom_Enums.generated.h"

UCLASS()
class DARKSOUL_API ACustom_Enums : public AActor
{
	GENERATED_BODY()
};

UENUM(BlueprintType)
enum class EStateType : uint8
{
	E_Idle = 0 					UMETA(DisplayName = "Idle"),
	E_Jumping					UMETA(DisplayName = "Jumping"),
	E_Attacking 				UMETA(DisplayName = "Attacking"),
	E_Skilling					UMETA(DisplayName = "Skilling"),
	E_Guarding					UMETA(DisplayName = "Guarding"),
	E_Vaulting_DoubleJump		UMETA(DisplayName = "Vaulting_DoubleJump"),
	E_Vaulting_Parkour			UMETA(DisplayName = "Vaulting_Parkour"),
	E_Vaulting_Climb			UMETA(DisplayName = "Vaulting_Climb"),
	E_Stumbling 				UMETA(DisplayName = "Stumbling"),
	E_Rolling 					UMETA(DisplayName = "Rolling"),
	E_KnockDown					UMETA(DisplayName = "Knock Down"),
	E_WakeUp					UMETA(DisplayName = "Wake Up"),
	E_Dead						UMETA(DisplayName = "Dead"),
	E_Max						UMETA(DisplayName = "Max"),
};

UENUM(BlueprintType)
enum class EComboType : uint8
{
	E_ComboType_1 = 0	UMETA(DisplayName = "First Combo Type"),
	E_ComboType_2 		UMETA(DisplayName = "Second Combo Type"),
	E_NoCombo			UMETA(DisplayName = "No Combo Type"),
	E_Max				UMETA(DisplayName = "Max"),
};

UENUM(BlueprintType)
enum class EPlayerCombatType : uint8
{
	E_Unarmed = 0	 	UMETA(DisplayName = "Unarmed"),
	E_Fighter 			UMETA(DisplayName = "Fighter"),
	E_SwordMaster 		UMETA(DisplayName = "Sword Master"),
	E_AxeMaster 		UMETA(DisplayName = "Axe Master"),
	E_Samurai			UMETA(DisplayName = "Samurai"),
	E_Warrior			UMETA(DisplayName = "Warrior"),
	E_Wizard 			UMETA(DisplayName = "Wizard"),
	E_Gunner 			UMETA(DisplayName = "Gunner"),
	E_Max				UMETA(DisplayName = "Max"),
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	E_Unarmed =0		UMETA(DisplayName = "Unarmed"),
	E_Fist				UMETA(DisplayName = "Fist"),
	E_Sword				UMETA(DisplayName = "Sword"),
	E_Axe				UMETA(DisplayName = "Axe"),
	E_Katana			UMETA(DisplayName = "Katana"),
	E_SwordAndShield	UMETA(DisplayName = "SwordAndShield"),
	E_Magic				UMETA(DisplayName = "Magic"),
	E_CastleKnight_Axe	UMETA(DisplayName = "CastleKnight_Axe"),
	E_Ghost_Axe			UMETA(DisplayName = "Ghost_Axe"),
	E_Bow				UMETA(DisplayName = "Bow"),
	E_DemonFist			UMETA(DisplayName = "Demon Fist"),
	E_Max				UMETA(DisplayName = "Max"),
};

UENUM(BlueprintType)
enum class EProjectileType : uint8
{
	E_MagicBall = 0 	UMETA(DisplayName = "Magic Ball"),
	E_SkullBall 		UMETA(DisplayName = "Skull Ball"),
	E_Ray				UMETA(DisplayName = "Ray"),
	E_Tornado			UMETA(DisplayName = "Tornado"),
	E_Arrow				UMETA(DisplayName = "Arrow"),
	E_Max				UMETA(DisplayName = "Max"),
};

UENUM(BlueprintType)
enum class EAttributeType : uint8
{
	E_Fire	= 0			UMETA(DisplayName = "Fire"),
	E_Ice				UMETA(DisplayName = "Ice"),
	E_Electric			UMETA(DisplayName = "Electric"),
	E_Dark				UMETA(DisplayName = "Dark"),
	E_Steel 			UMETA(DisplayName = "Steel"),
	E_Holy				UMETA(DisplayName = "Holy"),
	E_Max				UMETA(DisplayName = "Max"),
};

UENUM(BlueprintType)
enum class EEnemyType : uint8
{
	E_None = 0			UMETA(DisplayName = "None"),
	E_CastleKnight		UMETA(DisplayName = "Castle Knight"),
	E_CastleArchery		UMETA(DisplayName = "Castle Archery"),
	E_Demon				UMETA(DisplayName = "Demon"),
	E_Sevarog			UMETA(DisplayName = "Sevarog"),
	E_Max				UMETA(DisplayName = "Max"),
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	E_All = 0			UMETA(DisplayName = "All"),
	E_Potion 			UMETA(DisplayName = "Potion"),
	E_Weapon			UMETA(DisplayName = "Weapon"),
	E_Max
};

UENUM(BlueprintType)
enum class EWeaponRank : uint8
{
	E_Normal = 0		UMETA(DisplayName = "Normal"),
	E_Epic				UMETA(DisplayName = "Epic"),
	E_Legend			UMETA(DisplayName = "Legend"),
};

// AI	**************************************************

UENUM(BlueprintType)
enum class EAIState : uint8
{

	E_Idle = 0			UMETA(DisplayName = "Idle"),
	E_Investigate 		UMETA(DisplayName = "Investigate"),
	E_CoverFire			UMETA(DisplayName = "ComverFire"),
	E_Attack	 		UMETA(DisplayName = "Attack"),

};

UENUM(BlueprintType)
enum class EAICombatState : uint8
{
	E_NonCombat			UMETA(DisplayName = "Non Combat"),
	E_Combat			UMETA(DisplayName = "Combat"),
};

UENUM(BlueprintType)
enum class EAICombatPhase : uint8
{
	E_Phase1			UMETA(DisplayName = "Phase 1"),
	E_Phase2			UMETA(DisplayName = "Phase 2"),
	E_Phase3			UMETA(DisplayName = "Phase 3"),
	E_Max		
};

UENUM(BlueprintType)
enum class EFaction : uint8
{

	E_Enemy =0			UMETA(DisplayName = "Enemy"),
	E_Friendly			UMETA(DisplayName = "Friendly"),
	E_Neutral			UMETA(DisplayName = "Neutral"),

};

UENUM(BlueprintType)
enum class EStaticsOnStatusEffect : uint8
{

	E_Power = 0			UMETA(DisplayName = "Power"),
	E_Defense			UMETA(DisplayName = "Defense"),
	E_Speed				UMETA(DisplayName = "Speed"),
	E_Max		

};