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

	목적:
		Enemy Detectio Collider 내부에 위치한 Nearby Actor를 "시계 방향"으로 순회합니다.
	설명:
		1. 현재 Target Enemy로 설정된 Actor는 제외합니다.
		2. 현재 Target Enemy의 Key 값보다 큰 거리 값 중 가장 작은 수를 찾습니다.

******************************************************************************************/

void UC_TargetComponent::CycleClockwise()
{

	if (TargetingLockOn == true)
	{
		// TODO	:	양수 중 최소 값 찾기	
		float MinDistance = 100000.f;
		AGameObject* TmpEnemy = TargetEnemy;
		for (const auto& pair : MBoundEnemies)
		{
			// #1. Target Enemy 제외
			if (pair.Key == TargetEnemy) continue;

			// #2. 현 Target Enemy 거리보다 큰 거리 값 중 최소 
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

	목적:
		Enemy Detectio Collider 내부에 위치한 Nearby Actor를 "반 시계 방향"으로 순회합니다.
	설명:
		1. 현재 Target Enemy로 설정된 Actor는 제외합니다.
		2. 현재 Target Enemy의 Key 값보다 작은 거리 값 중 가장 큰 수를 찾습니다.

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

	목적:
		Enemy Detection Collider 범위 안에 존재하는 Nearby Enemy들을 찾아냅니다.
	설명:
		1. Nearby Actor가 1명이라도 존재하면, FindTargetEnemy() 함수가 호출됩니다.

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

	목적:
		Enemy Detection Collider에 Bouding된 주변 적들과 Player 간의 상대적인 거리 계산합니다.
		최종적으로, Min Distance(최소 거리)을 갖는 Enemy Actor가 Target Enemy로 선정됩니다.
	설명:
		1. A = (Player위치 - Enemy위치) X (Player의 Forward Vector)
		2. Min Distance = Fabs( (Player의 Up Vector) * (A) )
		3. Enemy & Min Distance 조합으로 Map을 생성합니다!

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

		// 최소 값 찾기
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
*	[Remark]	: Targeting 선형 보간
*
*	목적:
*		Target Lock이 켜져 있을 때, Target Enemy로 선정된  Actor를 바라보도록 합니다.
*	설명:
*		#1 Player Location과 Enemy Location을 활용하여 LookAtRotation 값을 구합니다.
*		#2 Camera의 지나치게 빠른 최전으로 어지러움 증상을 호소하기 이전에 RInterpTo 를 통해 선형 보간 적용 
*		#3 Controller Rotation을 위에서 구한 회전 값으로 설정합니다.
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

