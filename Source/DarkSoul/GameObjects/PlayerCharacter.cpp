#include "PlayerCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"

#include "ActorComponents/C_StateComponent.h"
#include "ActorComponents/C_HealthComponent.h"
#include "ActorComponents/C_ClassComponent.h"
#include "ActorComponents/C_StateComponent.h"
#include "ActorComponents/C_WeaponComponent.h"
#include "ActorComponents/C_TargetComponent.h"
#include "ActorComponents/C_ManaComponent.h"
#include "ActorComponents/C_HUDComponent.h"
#include "ActorComponents/C_InventoryComponent.h"

#include "Camera/CameraComponent.h"
#include "Camera/CameraShakeBase.h"
#include "Camera/CameraShake.h"

#include "AIController.h"
#include "BrainComponent.h"

#include "Space/Portal.h"

#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"


APlayerCharacter::APlayerCharacter()
{

	PrimaryActorTick.bCanEverTick = false;

	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(64.f, 96.f);

	// Skeletal Mesh
	GetMesh()->SetRelativeScale3D(FVector(1.05f, 1.05f, 1.05f));

	// Character Movement
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 300.f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.0f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// Rotation
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Spring Arm
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SprintArm"));

	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 500.f;
	SpringArm->SetRelativeRotation(FRotator(0.f, -20.f, 0.f));
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bDoCollisionTest = true;

	// ActorComponents
	ClassComponent = CreateDefaultSubobject<UC_ClassComponent>(TEXT("Class Component"));
	TargetComponent = CreateDefaultSubobject<UC_TargetComponent>(TEXT("Target Component"));
	HUDComponent = CreateDefaultSubobject<UC_HUDComponent>(TEXT("HUD Component"));
	InventoryComponent = CreateDefaultSubobject<UC_InventoryComponent>(TEXT("Inventory Component"));

	// Anim Instance
	TSubclassOf<UAnimInstance> animInstance;
	CHelpers::GetClass<UAnimInstance>(&animInstance, "AnimBlueprint'/Game/Blueprints/PlayerCharacter/ABP_PlayerCharacter'");
	if(animInstance)
	GetMesh()->SetAnimInstanceClass(animInstance);

	// Faction
	Faction = EFaction::E_Friendly;

}

void APlayerCharacter::BeginPlay()
{

	Super::BeginPlay();

	// #0. Materials
	UMaterialInstanceConstant* outfit;
	UMaterialInstanceConstant* head;
	UMaterialInstanceConstant* body;
	UMaterialInstanceConstant* hair;

	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&outfit, "MaterialInstanceConstant'/Game/GladiatorWarrior/Materials/MI_outfit.MI_outfit'");
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&head, "MaterialInstanceConstant'/Game/GladiatorWarrior/Materials/MI_head.MI_head'");
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&body, "MaterialInstanceConstant'/Game/GladiatorWarrior/Materials/MI_body.MI_body'");
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&hair, "MaterialInstanceConstant'/Game/GladiatorWarrior/Materials/MI_hair.MI_hair'");

	OutfitMaterial = UMaterialInstanceDynamic::Create(outfit, this);
	HeadMaterial = UMaterialInstanceDynamic::Create(head, this);
	BodyMaterial = UMaterialInstanceDynamic::Create(body, this);
	HairMaterial = UMaterialInstanceDynamic::Create(hair, this);

	// #2. Inventory 버그 보정
	InventoryComponent->ShowInventoryWidgetOnScreen(EItemType::E_All);
	InventoryComponent->ShowInventoryWidgetOnScreen(EItemType::E_Weapon);
	InventoryComponent->ShowInventoryWidgetOnScreen(EItemType::E_Potion);

	InventoryComponent->RemoveInventoryWidgetOnScreen();

	//	Temp
	if(WeaponComponent)
	{
		OnChangeWeapon.ExecuteIfBound(EWeaponType::E_Axe);
		OnChangeClass.ExecuteIfBound(EPlayerCombatType::E_AxeMaster);
	}


}


void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("CycleClockwise+", IE_Pressed, this,
		&APlayerCharacter::CycleClockwise);
	PlayerInputComponent->BindAction("CycleClockwise-", IE_Pressed, this,
		&APlayerCharacter::CycleAntiClockwise);

	PlayerInputComponent->BindAction("Jumping", IE_Pressed, this,
		&AGameObject::StartJump);

	PlayerInputComponent->BindAction("TargetLockOn", IE_Pressed, this,
		&APlayerCharacter::LockOnTarget);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this,
		&APlayerCharacter::Attack);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this,
		&AGameObject::OnSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this,
		&AGameObject::OffSprint);
	PlayerInputComponent->BindAction("Roll", IE_Pressed, this,
		&APlayerCharacter::StartRoll);
	PlayerInputComponent->BindAction("EquipFist", IE_Pressed, this,
		&APlayerCharacter::EquipFist);
	PlayerInputComponent->BindAction("EquipSword", IE_Pressed, this,
		&APlayerCharacter::EquipSword);
	PlayerInputComponent->BindAction("EquipAxe", IE_Pressed, this,
		&APlayerCharacter::EquipAxe);
	PlayerInputComponent->BindAction("EquipKatana", IE_Pressed, this,
		&APlayerCharacter::EquipKatana);
	PlayerInputComponent->BindAction("EquipSwordAndShield", IE_Pressed, this,
		&APlayerCharacter::EquipSwordAndShield);
	PlayerInputComponent->BindAction("EquipWizard", IE_Pressed, this,
		&APlayerCharacter::EquipWizard);
	PlayerInputComponent->BindAction("ChangeComboType", IE_Pressed, this,
		&APlayerCharacter::ChangePlayerComboType);
	PlayerInputComponent->BindAction("Skill00", IE_Pressed, this,
		&AGameObject::AttackSkill_00);
	PlayerInputComponent->BindAction("Skill01", IE_Pressed, this,
		&AGameObject::AttackSkill_01);
	PlayerInputComponent->BindAction("Skill02", IE_Pressed, this,
		&AGameObject::AttackSkill_02);
	PlayerInputComponent->BindAction("Guard", IE_Pressed, this,
		&APlayerCharacter::GuardSkill);
	PlayerInputComponent->BindAction("Vault", IE_Pressed, this,
		&AGameObject::StartVault);
	PlayerInputComponent->BindAction("OpenInventory", IE_Pressed, this,
		&APlayerCharacter::OpenInventory);
	PlayerInputComponent->BindAction("Acquire", IE_Pressed, this,
		&APlayerCharacter::AcquireItem);
	PlayerInputComponent->BindAction("OnCompass", IE_Pressed, this,
		&APlayerCharacter::OnCompass);
	PlayerInputComponent->BindAction("ChangeLevel", IE_Pressed, this,
		&APlayerCharacter::OnChangeLevel);


}

void APlayerCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) 
		&& (Value != 0.f) 
		&& StateComponent->GetStateType() == EStateType::E_Idle
		&& (GetCharacterMovement()->IsFalling() == false))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(Direction, Value);
	}

}

void APlayerCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) 
		&& (Value != 0.f) 
		&& StateComponent->GetStateType() == EStateType::E_Idle 
		&& (GetCharacterMovement()->IsFalling() == false))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, Value);
	}
}

void APlayerCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}


// [23-07-21] : Change Color
void APlayerCharacter::ChangeColor()
{

	Super::ChangeColor();

	if(GetMesh()->GetNumMaterials() > 0)
	{
		if (!!BloodMaterial)
		{
			for (int32 i = 0; i < 5; i++)
			{
				if (!!GetMesh()->GetMaterial(i))
				{
					GetMesh()->SetMaterial(i, BloodMaterial);
				}
			}
			GetWorld()->GetTimerManager().SetTimer(ChangeMaterialHandler, FTimerDelegate::CreateLambda([&]() {
				RestoreColor();
				}), 0.1f, false);
		}
	}

}


// [23-07-21] : Change Color
void APlayerCharacter::RestoreColor()
{
	
	Super::RestoreColor();

	if (!!OutfitMaterial && !!HeadMaterial && !!BodyMaterial && !!HairMaterial
		&& GetMesh()->GetMaterial(0) && GetMesh()->GetMaterial(1) && GetMesh()->GetMaterial(2) && GetMesh()->GetMaterial(3) && GetMesh()->GetMaterial(4))
	{
		GetMesh()->SetMaterial(0, OutfitMaterial);
		GetMesh()->SetMaterial(1, HeadMaterial);
		GetMesh()->SetMaterial(2, BodyMaterial);
		GetMesh()->SetMaterial(3, OutfitMaterial);
		GetMesh()->SetMaterial(4, HairMaterial);
	}


}


void APlayerCharacter::StartRoll()
{

	CheckTrue(!StateComponent);

	if (StateComponent->GetStateType() == EStateType::E_Idle
		&& WeaponComponent->GetWeaponType() != EWeaponType::E_Unarmed
		&& GetCharacterMovement()->IsFalling() == false)
	{
		OnChangeStateType.ExecuteIfBound(EStateType::E_Rolling);
		OnRoll.ExecuteIfBound();
	}

}

void APlayerCharacter::EndRoll()
{

	OnChangeStateType.ExecuteIfBound(EStateType::E_Idle);

	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

}

void APlayerCharacter::EquipFist()
{

	CheckTrue(!StateComponent);

	if (StateComponent->GetStateType() == EStateType::E_Idle
		&& GetCharacterMovement()->IsFalling() != true)
	{

		OnChangeWeapon.ExecuteIfBound(EWeaponType::E_Fist);

		OnChangeClass.ExecuteIfBound(EPlayerCombatType::E_Fighter);
	}

}

void APlayerCharacter::EquipSword()
{
	CheckNull(StateComponent);

	if (StateComponent->GetStateType() == EStateType::E_Idle
		&& GetCharacterMovement()->IsFalling() != true)
	{
		OnChangeWeapon.ExecuteIfBound(EWeaponType::E_Sword);

		OnChangeClass.ExecuteIfBound(EPlayerCombatType::E_SwordMaster);
	}

}

void APlayerCharacter::EquipAxe()
{

	CheckNull(StateComponent);

	if (StateComponent->GetStateType() == EStateType::E_Idle
		&& GetCharacterMovement()->IsFalling() != true)
	{
		OnChangeWeapon.ExecuteIfBound(EWeaponType::E_Axe);

		OnChangeClass.ExecuteIfBound(EPlayerCombatType::E_AxeMaster);
	}

}


void APlayerCharacter::EquipKatana()
{

	CheckNull(StateComponent);

	if (StateComponent->GetStateType() == EStateType::E_Idle
		&& GetCharacterMovement()->IsFalling() != true)
	{
		OnChangeWeapon.ExecuteIfBound(EWeaponType::E_Katana);

		OnChangeClass.ExecuteIfBound(EPlayerCombatType::E_Samurai);
	}

}

void APlayerCharacter::EquipSwordAndShield()
{

	CheckNull(StateComponent);

	if (StateComponent->GetStateType() == EStateType::E_Idle
		&& GetCharacterMovement()->IsFalling() != true)
	{
		OnChangeWeapon.ExecuteIfBound(EWeaponType::E_SwordAndShield);

		OnChangeClass.ExecuteIfBound(EPlayerCombatType::E_SwordMaster);
	}

}

void APlayerCharacter::EquipWizard()
{

	CheckNull(StateComponent);

	if (StateComponent->GetStateType() == EStateType::E_Idle
		&& GetCharacterMovement()->IsFalling() != true)
	{
		OnChangeWeapon.ExecuteIfBound(EWeaponType::E_Magic);

		OnChangeClass.ExecuteIfBound(EPlayerCombatType::E_Wizard);
	}

}


void APlayerCharacter::ChangePlayerComboType()
{

	CheckTrue(!WeaponComponent);

	uint8 CurrentComboType = static_cast<int>(WeaponComponent->GetCurrentWeapon()->GetComboType());
	WeaponComponent->ChangeComboType(static_cast<EComboType>((++CurrentComboType) % 2));

}


void APlayerCharacter::LockOnTarget()
{

	OnLockOnTarget.ExecuteIfBound();

}


void APlayerCharacter::CycleClockwise()
{

	OnCycleClockwise.ExecuteIfBound();

}


void APlayerCharacter::CycleAntiClockwise()
{

	OnCycleAntiClockwise.ExecuteIfBound();

}

void APlayerCharacter::OpenInventory()
{
	CheckTrue(!InventoryComponent);

	if (InventoryComponent->IsInventoryShownOnScreen() == false)
	{
		OnShowInventoryWidget.ExecuteIfBound(EItemType::E_All);
	}
	else
	{
		OnRemoveInventoryWidget.ExecuteIfBound();
	}

}

void APlayerCharacter::AcquireItem()
{

	CheckTrue(!StateComponent);

	if (StateComponent->GetStateType() == EStateType::E_Idle)
	{
		OnAcquireItem.ExecuteIfBound();
	}


}

void APlayerCharacter::OnCompass()
{

	OnShowCompassWidget.ExecuteIfBound();

}


void APlayerCharacter::OnChangeLevel()
{

	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, APortal::StaticClass());

	for (const auto OverlappingActor : OverlappingActors)
	{
		APortal* Portal = Cast<APortal>(OverlappingActor);
		if (!!OverlappingActor && !!Portal)
		{
			OnSwitchLevel.Broadcast(this);
		}
	}

}
