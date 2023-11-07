#include "C_TargetComponent.h"
#include "Global/Global.h"
#include "ActorComponents/C_StateComponent.h"

#include "GameObjects/GameObject.h"

UC_TargetComponent::UC_TargetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UC_TargetComponent::BeginPlay()
{

	Super::BeginPlay();

	OwnerObject = Cast<AGameObject>(GetOwner());
	CheckTrue(!OwnerObject);

	OwnerObject->OnLockOnTarget.BindUFunction(this, "LockOnTarget");
	OwnerObject->OnCycleClockwise.BindUFunction(this, "CycleClockwise");
	OwnerObject->OnCycleAntiClockwise.BindUFunction(this, "CycleAntiClockwise");

}

void UC_TargetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TargetingLockOn == false)
	{
		FindNearbyActors();
	}
	else
	{
		CheckNull(TargetEnemy);

		if (TargetEnemy->GetStateComponent()->GetStateType() == EStateType::E_Dead)
			LockOnTarget();
		else
			FocusOnTarget(DeltaTime);
	}

}

void UC_TargetComponent::LockOnTarget()
{
	if (TargetingLockOn == false)
	{
		CheckNull(TargetEnemy);
		TargetingLockOn = true;
	}
	else
	{
		TargetingLockOn = false;
		MBoundEnemies.Empty();
	}
}

/*****************************************************************************************

	����:
		Enemy Detectio Collider ���ο� ��ġ�� Nearby Actor�� "�ð� ����"���� ��ȸ�մϴ�.
	����:
		1. ���� Target Enemy�� ������ Actor�� �����մϴ�.
		2. ���� Target Enemy�� Key ������ ū �Ÿ� �� �� ���� ���� ���� ã���ϴ�.

******************************************************************************************/

void UC_TargetComponent::CycleClockwise()
{

	if (TargetingLockOn == true)
	{
		// TODO	:	��� �� �ּ� �� ã��	
		float MinDistance = 100000.f;
		AGameObject* TmpEnemy = TargetEnemy;
		for (const auto& pair : MBoundEnemies)
		{
			// #1. Target Enemy ����
			if (pair.Key == TargetEnemy) continue;

			// #2. �� Target Enemy �Ÿ����� ū �Ÿ� �� �� �ּ� 
			if (pair.Value > *MBoundEnemies.Find(TargetEnemy)
				&& pair.Value < MinDistance)
			{
				MinDistance = pair.Value;
				TmpEnemy = pair.Key;
			}
		}

		CheckNull(TmpEnemy);
		TargetEnemy = TmpEnemy;

	}

}

/*****************************************************************************************

	����:
		Enemy Detectio Collider ���ο� ��ġ�� Nearby Actor�� "�� �ð� ����"���� ��ȸ�մϴ�.
	����:
		1. ���� Target Enemy�� ������ Actor�� �����մϴ�.
		2. ���� Target Enemy�� Key ������ ���� �Ÿ� �� �� ���� ū ���� ã���ϴ�.

******************************************************************************************/

void UC_TargetComponent::CycleAntiClockwise()
{

	if (TargetingLockOn == true)
	{

		float MaxDistance = -99999.f;
		AGameObject* TmpEnemy = TargetEnemy;
		for (const auto& pair : MBoundEnemies)
		{

			if (pair.Key == TargetEnemy) continue;

			if (pair.Value < *MBoundEnemies.Find(TargetEnemy)
				&& pair.Value > MaxDistance)
			{
				MaxDistance = pair.Value;
				TmpEnemy = pair.Key;
			}
		}

		CheckNull(TmpEnemy);
		TargetEnemy = TmpEnemy;

	}

}

/*****************************************************************************************

	����:
		Enemy Detection Collider ���� �ȿ� �����ϴ� Nearby Enemy���� ã�Ƴ��ϴ�.
	����:
		1. Nearby Actor�� 1���̶� �����ϸ�, FindTargetEnemy() �Լ��� ȣ��˴ϴ�.

******************************************************************************************/

void UC_TargetComponent::FindNearbyActors()
{
	TArray<TEnumAsByte<EObjectTypeQuery>> NearbyActors;
	TEnumAsByte<EObjectTypeQuery> PawnObjectType = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn);
	NearbyActors.Add(PawnObjectType);

	AActor* OwnerActor = GetOwner();
	CheckNull(OwnerActor);

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(OwnerActor);

	TArray<FHitResult> HitResults;

	// Trace Multi For Objects
	bool Result = UKismetSystemLibrary::SphereTraceMultiForObjects(
		GetWorld(),
		OwnerActor->GetActorLocation(),
		OwnerActor->GetActorLocation() + 10.f,
		EnemyDetectionMaxRadius,
		NearbyActors,
		false,
		IgnoreActors,
		EDrawDebugTrace::None,
		HitResults,
		true
	);

	// When Enemy Detecetd!
	if (Result == true)
	{
		// TODO	:	Decide Targeting Enemy
		for (const auto& Hit : HitResults)
		{
			if (Cast<AGameObject>(Hit.GetActor()) != nullptr)
			{
				NearbyEnemies.AddUnique(Cast<AGameObject>(Hit.GetActor()));
			}
		}
		
		if (NearbyActors.IsValidIndex(0) == false) return;
		
		FindTargetEnemy();
	}

}

/*****************************************************************************************

	����:
		Enemy Detection Collider�� Bouding�� �ֺ� ����� Player ���� ������� �Ÿ� ����մϴ�.
		����������, Min Distance(�ּ� �Ÿ�)�� ���� Enemy Actor�� Target Enemy�� �����˴ϴ�.
	����:
		1. A = (Player��ġ - Enemy��ġ) X (Player�� Forward Vector)
		2. Min Distance = Fabs( (Player�� Up Vector) * (A) )
		3. Enemy & Min Distance �������� Map�� �����մϴ�!

******************************************************************************************/

void UC_TargetComponent::FindTargetEnemy()
{

	AActor* OwnerActor = GetOwner();
	CheckNull(OwnerActor);
	float MinDistance = 100000.f;

	FVector TempCrossProduct = FVector::ZeroVector;
	float TempDotProduct = 0.f;
	int32 TargetEnemyIndex = -1;

	for (int i = 0; i < NearbyEnemies.Num(); i++)
	{
		// Cross + Dot
		TempCrossProduct
			= FVector::CrossProduct(OwnerActor->GetActorForwardVector(), NearbyEnemies[i]->GetActorLocation() - OwnerActor->GetActorLocation());
		TempDotProduct
			= FVector::DotProduct(OwnerActor->GetActorUpVector(), TempCrossProduct);

		// Map
		MBoundEnemies.Add(NearbyEnemies[i], TempDotProduct);

		// �ּ� �� ã��
		float Distance = fabs(TempDotProduct);
		if (Distance < MinDistance)
		{
			MinDistance = Distance;
			TargetEnemyIndex = i;
		}
	}

	if (TargetEnemyIndex != -1)
	{
		TargetEnemy = NearbyEnemies[TargetEnemyIndex];
		CheckNull(TargetEnemy);
	}
}

/*	****************************************************************************************
* 
*	[Remark]	: Targeting ���� ����
*
*	����:
*		Target Lock�� ���� ���� ��, Target Enemy�� ������  Actor�� �ٶ󺸵��� �մϴ�.
*	����:
*		#1 Player Location�� Enemy Location�� Ȱ���Ͽ� LookAtRotation ���� ���մϴ�.
*		#2 Camera�� ����ġ�� ���� �������� �������� ������ ȣ���ϱ� ������ RInterpTo �� ���� ���� ���� ���� 
*		#3 Controller Rotation�� ������ ���� ȸ�� ������ �����մϴ�.
*		
*	****************************************************************************************/

void UC_TargetComponent::FocusOnTarget(float DeltaTime)
{

	AActor* OwnerActor = GetOwner();
	CheckNull(OwnerActor);

	ACharacter* OwnerCharacter = Cast<ACharacter>(OwnerActor);
	CheckNull(OwnerCharacter);

	CheckFalse(TargetingLockOn);
	CheckNull(TargetEnemy);

	// #1. FindLookAtRotation
	APlayerCameraManager* CameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	FVector Start = CameraManager->GetCameraLocation();
	FVector Target = TargetEnemy->GetActorLocation();

	// #2. Interpolation
	FRotator StartRotation = OwnerCharacter->GetControlRotation();
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(Start, Target);

	FRotator FinalRotation = UKismetMathLibrary::RInterpTo(StartRotation, TargetRotation, DeltaTime, InteropSpeed);

	OwnerCharacter->GetController()->SetControlRotation(FRotator(0.f, FinalRotation.Yaw, 0.f));


}

