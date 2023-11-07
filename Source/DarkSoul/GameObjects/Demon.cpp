#include "Demon.h"
#include "Global/Global.h"

#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"

#include "ActorComponents/C_ManaComponent.h"
#include "ActorComponents/C_StateComponent.h"
#include "ActorComponents/C_WeaponComponent.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "AIController/AIC_EnemyBase.h"
#include "BrainComponent.h"

#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"


ADemon::ADemon()
{

	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->InitCapsuleSize(100.f, 150.f);

	GetMesh()->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -128.f));

	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.0f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	HUDComponent->SetRelativeScale3D(FVector(8.f, 8.f, 8.f));
	HUDComponent->SetRelativeLocation(FVector(0.f, 0.f, 300.f));

	// Animation Blueprint
	TSubclassOf<UAnimInstance> animInstance;
	CHelpers::GetClass<UAnimInstance>(&animInstance, "AnimBlueprint'/Game/Blueprints/Enemy/Demon/ABP_Demon'");
	GetMesh()->SetAnimInstanceClass(animInstance);

}

void ADemon::BeginPlay()
{

	Super::BeginPlay();

	// #1. Materials
	UMaterialInstanceConstant* body;
	UMaterialInstanceConstant* wing;
	UMaterialInstanceConstant* blood;

	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&body, "MaterialInstanceConstant'/Game/BigDemon/Materials/Mi_BigDemon_Body_Skin2.MI_BigDemon_Body_Skin2'");
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&wing, "MaterialInstanceConstant'/Game/BigDemon/Materials/MI_BigDemon_Wings_Skin2.MI_BigDemon_Wings_Skin2'");
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&blood, "MaterialInstanceConstant'/Game/mi_blood_water_bright.mi_blood_water_bright'");

	BodyMaterial = UMaterialInstanceDynamic::Create(body, this);
	WingMaterial = UMaterialInstanceDynamic::Create(wing, this);
	BloodMaterial = UMaterialInstanceDynamic::Create(blood, this);

}

void ADemon::IsStartDamaged(AActor* InDamageCauser, float InImpulseAmount, bool bStatusEffectTriggered)
{

	ACharacter* DamageCauserCharacter = Cast<ACharacter>(InDamageCauser->GetOwner());
	CheckTrue(!DamageCauserCharacter);

	// #0. Target Rotation
	LookAtDamageCauser(&DamageCauserCharacter);

	// #0. State
	OnChangeStateType.ExecuteIfBound(EStateType::E_Stumbling);
	
	// #1. Sound Comp
	OnPlayHumanSound.ExecuteIfBound(
		this,
		StateComponent->GetStateType(),
		WeaponComponent->GetWeaponType(),
		WeaponComponent->GetCurrentWeapon()->GetComboType(),
		-1,
		-1
	);

	// #2. AI
	if(!!AIController) 
		AIController->GetBrainComponent()->PauseLogic("Agent Is Stumbling");

	// #3. Change Material
	//if(!bStatusEffectTriggered)
	//	ChangeColor();

	// #3. End Damaged
	FTimerHandle EndDamageHandler;
	GetWorld()->GetTimerManager().SetTimer(EndDamageHandler, FTimerDelegate::CreateLambda([&]() {
		IsEndDamaged();
		}), 1.5f, false);

}

void ADemon::IsEndDamaged()
{
	Super::IsEndDamaged();
}

void ADemon::ChangeColor()
{

	if (!!BloodMaterial && !!GetMesh()->GetMaterial(2))
	{
		Super::ChangeColor();
		GetMesh()->SetMaterial(2, BloodMaterial);
		GetWorld()->GetTimerManager().SetTimer(ChangeMaterialHandler, FTimerDelegate::CreateLambda([&]() {
			RestoreColor();
			}), 0.1f, false);
	}
	else
	{
		PrintLine();
	}

}

void ADemon::RestoreColor()
{
	
	Super::RestoreColor();

	if (!!BodyMaterial && !!WingMaterial 
		&& !!GetMesh()->GetMaterial(0) && !!GetMesh()->GetMaterial(2))
	{
		GetMesh()->SetMaterial(0, BodyMaterial);
		GetMesh()->SetMaterial(2, WingMaterial);
	}
	else
	{
		PrintLine();
	}

}

void ADemon::RunAICombatPhase(EAICombatPhase InEnum)
{

	Super::RunAICombatPhase(InEnum);

	switch (InEnum)
	{
	case EAICombatPhase::E_Phase1:
		AICombatPhase01();
		break;
	case EAICombatPhase::E_Phase2:
		AICombatPhase02();
		break;
	case EAICombatPhase::E_Phase3:
		AICombatPhase03();
		break;
	}

}


//	[23-05-03] Remark : AIPhase
void ADemon::AICombatPhase01()
{

	if (GetManaComponent()->GetAttackSillValid(1)) AttackSkill_01();

}

void ADemon::AICombatPhase02()
{

	if (GetManaComponent()->GetAttackSillValid(1))
	{
		AttackSkill_01();
	}
	else if(GetManaComponent()->GetAttackSillValid(0))
	{
		AttackSkill_00();
	}

}

void ADemon::AICombatPhase03()
{
	if (GetManaComponent()->GetAttackSillValid(2))
	{
		AttackSkill_02();
	}
	else if (GetManaComponent()->GetAttackSillValid(0))
	{
		AttackSkill_00();
	}
}
