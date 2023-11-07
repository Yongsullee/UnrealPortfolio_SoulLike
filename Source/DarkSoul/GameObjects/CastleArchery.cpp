#include "CastleArchery.h"
#include "Global/Global.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "ActorComponents/C_WeaponComponent.h"

#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"

#include "AssetRegistryModule.h"

ACastleArchery::ACastleArchery()
{

	//PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->InitCapsuleSize(42.0f, 100.f);
	GetMesh()->SetRelativeRotation(FRotator(0.f, 0.f, -90.f));
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -100.f));
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.0f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Animation Blueprint
	TSubclassOf<UAnimInstance> animInstance;
	CHelpers::GetClass<UAnimInstance>(&animInstance, "AnimBlueprint'/Game/Blueprints/Enemy/CastleArchery/ABP_CastleArchery'");
	GetMesh()->SetAnimInstanceClass(animInstance);

}

void ACastleArchery::BeginPlay()
{

	Super::BeginPlay();

	// [23-07-21] Change Material
	// #1. Materials
	UMaterialInstanceConstant* body;
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&body, "MaterialInstanceConstant'/Game/Assets/Assets_Infinity_Characters/Character/CompleteCharacters/Textures_Materials/CharM_Barbarous/Mi_Char_Barbrous.MI_Char_Barbrous'");
	BodyMaterial = UMaterialInstanceDynamic::Create(body, this);

	// #2. Temp
	if (WeaponComponent)
	{
		OnChangeWeapon.ExecuteIfBound(EWeaponType::E_Bow);
	}

}

void ACastleArchery::RestoreColor()
{
	
	Super::RestoreColor();

	if (!!BodyMaterial && !!GetMesh()->GetMaterial(0))
	{
		GetMesh()->SetMaterial(0, BodyMaterial);
	}
	else
	{
		PrintLine();
	}

}

void ACastleArchery::LookAtDamageCauser(ACharacter** InCharacter)
{

	if (!!(*InCharacter))
	{
		FVector TargetLocation = (*InCharacter)->GetActorLocation();
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLocation);
		//LookAtRotation += FRotator(0.f, 90.f, 0.f);

		SetActorRotation(LookAtRotation);
	}
	else
	{
		PrintLine();
	}

}
