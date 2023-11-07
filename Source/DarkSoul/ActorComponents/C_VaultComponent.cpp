#include "C_VaultComponent.h"
#include "Global/Global.h"

#include "GameObjects/GameObject.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"


UC_VaultComponent::UC_VaultComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UC_VaultComponent::BeginPlay()
{

	Super::BeginPlay();

	OwnerObject  = Cast<AGameObject>(GetOwner());
	CheckTrue(!OwnerObject);

	OwnerObject->OnRoll.BindUFunction(this, "Roll");
	OwnerObject->OnDoubleJump.BindUFunction(this, "DoubleJump");
	OwnerObject->OnStartVault.BindUFunction(this, "StartVault");

}

// TODO	:	Play Montage + Delay + Set Owner Location
void UC_VaultComponent::StartVault()
{

	CheckTrue(!OwnerObject);

	if (DetectWall(&OwnerObject) == true)
	{
		// 0. 점프 후 Vault 할 시
		if (bCanVault == true && OwnerObject->GetCharacterMovement()->IsFalling() == true)
		{
			bCanVault = false;
			bCanClimb = true;
		}

		FixRotation(&OwnerObject);

		OffCollision(&OwnerObject);

		float Duration = 0.f;
		if (bCanVault == true)
		{
			FixLocation(&OwnerObject, VaultAnimStartLocation, VaultAnimStartHeight);
			Duration = Vault(&OwnerObject);
		}
		else if (bCanClimb == true)
		{
			FixLocation(&OwnerObject, ClimbAnimStartLocation, ClimbAnimStartHeight);
			Duration = Climb(&OwnerObject);
		}

		EndVault(Duration);

	}

}

void UC_VaultComponent::EndVault(float InFloat)
{
	FTimerHandle TimerHandler;
	FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this, &UC_VaultComponent::OnCollision);
	GetWorld()->GetTimerManager().SetTimer(TimerHandler, TimerDelegate, InFloat*0.6f, false);

	bCanVault = false;
	bCanClimb = false;

}

float UC_VaultComponent::Vault(AGameObject** InPlayer)
{

	OnPlayParkourAnim.ExecuteIfBound(EStateType::E_Vaulting_Parkour, EWeaponType::E_Unarmed, EComboType::E_Max, -1, -1);

	float Duration = 1.3f;

	return Duration;
}

float UC_VaultComponent::Climb(AGameObject** InPlayer)
{

	OnPlayClimbAnim.ExecuteIfBound(EStateType::E_Vaulting_Climb, EWeaponType::E_Unarmed, EComboType::E_Max, -1, -1);

	float Duration = 1.3f;

	return Duration;

}

void UC_VaultComponent::Roll()
{

	CheckTrue(!OwnerObject);

	// #0. Montage

	// #1. Location + Rotation
	FVector Start = OwnerObject->GetActorLocation();
	FVector Target = OwnerObject->GetActorLocation() + OwnerObject->GetVelocity();
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(Start, Target);

	OwnerObject->bUseControllerRotationYaw = false;
	OwnerObject->GetCharacterMovement()->bOrientRotationToMovement = true;

	OwnerObject->SetActorRotation(LookAtRotation);

	OnPlayRollAnim.ExecuteIfBound(
		EStateType::E_Rolling, 
		EWeaponType::E_Unarmed, 
		EComboType::E_Max, 
		-1, 
		-1
	);

}

void UC_VaultComponent::DoubleJump()
{

	OwnerObject->LaunchCharacter(FVector(0.f, 0.f, 500.f), false, false);

	OnPlayDoubleJumpAnim.ExecuteIfBound(EStateType::E_Vaulting_DoubleJump, EWeaponType::E_Unarmed, EComboType::E_Max, -1, -1);

}

bool UC_VaultComponent::DetectWall(class AGameObject** InPlayer)
{

	bCanVault = true;

	bCanVault &= DetectWallFront(InPlayer);

	if (bCanVault == true)
		bCanVault &= DetectWallHeight(InPlayer);

	if (bCanVault == true || bCanClimb == true)
		bCanVault &= DetectWallDepth(InPlayer);

	return bCanVault || bCanClimb;
}

// TODO	:	LineTraceForObjects
bool UC_VaultComponent::DetectWallFront(AGameObject** InPlayer)
{

	// 0. Start + End Location
	FVector StartLocation = (*InPlayer)->GetVaultArrowComp()->GetComponentLocation();
	FVector EndLocation = StartLocation + ((*InPlayer)->GetVaultArrowComp()->GetForwardVector() * DistanceFrontWall);

	// 1. Object Types
	TArray<TEnumAsByte<EObjectTypeQuery>> NearbyStaticMesh;
	TEnumAsByte<EObjectTypeQuery> ObjecType = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic);
	NearbyStaticMesh.Add(ObjecType);

	// 2. Ignore Actors
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add((*InPlayer));

	// Front
	FHitResult FrontResult;
	bool Result = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		StartLocation,
		EndLocation,
		NearbyStaticMesh,
		false,
		IgnoreActors,
		EDrawDebugTrace::None,
		FrontResult,
		true
	);

	FrontWallLocation = FrontResult.Location;
	FrontWallNormalLocation = FrontResult.ImpactNormal;

	return Result;

}

// TODO	:	LineTraceForObjects
bool UC_VaultComponent::DetectWallHeight(AGameObject** InPlayer)
{

	// 0. Start + End Location
	FVector StartLocation = (UKismetMathLibrary::GetForwardVector(FrontWallNormalLocation.Rotation())) * -10.f + FrontWallLocation + FVector(0.f, 0.f, 200.f);
	FVector EndLocation = StartLocation - FVector(0.f, 0.f, 200.f);

	// 1. Object Types
	TArray<TEnumAsByte<EObjectTypeQuery>> NearbyStaticMesh;
	TEnumAsByte<EObjectTypeQuery> ObjecType = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic);
	NearbyStaticMesh.Add(ObjecType);

	// 2. Ignore Actors
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add((*InPlayer));

	// 3. Hit Result
	FHitResult HeightResult;
	bool Result = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		StartLocation,
		EndLocation,
		NearbyStaticMesh,
		false,
		IgnoreActors,
		EDrawDebugTrace::None,
		HeightResult,
		true
	);

	HeightWallLocation = HeightResult.Location;
	float Height = fabs(FrontWallLocation.Z - HeightWallLocation.Z);

	if (Height >= VaultMaxHeight)
	{
		Result = false;

		if(Height <= ClimbMaxHeight)
			bCanClimb = true;
	}


	return Result;
}

// TODO	:	LineTraceForObjects
bool UC_VaultComponent::DetectWallDepth(AGameObject** InPlayer)
{
	// 1. Object Types
	TArray<TEnumAsByte<EObjectTypeQuery>> NearbyStaticMesh;
	TEnumAsByte<EObjectTypeQuery> ObjecType = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic);
	NearbyStaticMesh.Add(ObjecType);

	// 2. Ignore Actors
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add((*InPlayer));

	FHitResult DepthResult;

	// 0. Start + End Location
	FVector StartLocation = (UKismetMathLibrary::GetForwardVector(FrontWallNormalLocation.Rotation())) * DistanceDepthWall + FrontWallLocation + FVector(0.f, 0.f, 300.f);
	FVector EndLocation = StartLocation - FVector(0.f, 0.f, 275.f);

	bool Result = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		StartLocation,
		EndLocation,
		NearbyStaticMesh,
		false,
		IgnoreActors,
		EDrawDebugTrace::None,
		DepthResult,
		true
	);

	// CanClimb?
	if (Result == false)
	{
		bCanClimb = false;
	}
	else
	{
		// Height Location - Depth Location
		if (fabs(FVector::DistSquared2D(DepthResult.Location, HeightWallLocation)) > 30.f)
		{
			DepthWallLocation = DepthResult.Location;
			bCanClimb = true;
		}
	}

	return !Result;
}

// TODO	:	Set Owner Rotation
void UC_VaultComponent::FixRotation(class AGameObject** InPlayer)
{
	FRotator Rotation = (*InPlayer)->GetActorRotation();
	FRotator ChangeRotation = FrontWallNormalLocation.Rotation();
	ChangeRotation.Yaw += 180.f;
	Rotation.Yaw = ChangeRotation.Yaw;

	(*InPlayer)->SetActorRotation(Rotation);
}

void UC_VaultComponent::FixLocation(AGameObject** InPlayer, float InFloat1, float InFloat2)
{
	FVector ChangeLocation = (UKismetMathLibrary::GetForwardVector(FrontWallNormalLocation.Rotation())) * InFloat1 + FrontWallLocation;
	float VectorZ = HeightWallLocation.Z + InFloat2;

	FVector NewLocation = FVector(ChangeLocation.X, ChangeLocation.Y, VectorZ);
	(*InPlayer)->SetActorLocation(NewLocation);
}

// TODO	:	Change Owner Collision + Movement Mode
void UC_VaultComponent::OnCollision()
{

	CheckTrue(!OwnerObject);

	OwnerObject->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	OwnerObject->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	
}

// TODO	:	Change Owner Collision + Movement Mode
void UC_VaultComponent::OffCollision(AGameObject** InPlayer)
{

	(*InPlayer)->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	(*InPlayer)->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);

}
