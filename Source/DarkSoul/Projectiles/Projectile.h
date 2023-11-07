#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Global/Global.h"

#include "Projectile.generated.h"

/*	*********************************************************************************************************************
* 
*	[Remark_23-03-20]
* 
*	목적: 충돌과 생명 주기 Delegate
*	
*	설명:
*		1. FProjectileBeginOverlap	: Projectile의 충돌 이벤트를 이에 동적 바인딩을 수행한 NoComboWeapon에 알립니다.
*		2. FProjectileEndLaunched	: Porjectile의 생명 주기 이벤트를 이에 동적 바인딩을 수행한 액터 컴포넌트에 알립니다.
* 
*	*********************************************************************************************************************/

DECLARE_DYNAMIC_DELEGATE_ThreeParams(FProjectileBeginOverlap, class AActor*, InActor, EProjectileType, InType, uint8, InIndex);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FProjectileEndLaunched, EProjectileType, InType, uint8, InIndex);

UCLASS()
class DARKSOUL_API AProjectile : public AActor
{

	GENERATED_BODY()
	
public:	
	AProjectile();

protected:
	virtual void BeginPlay() override;

protected:
	UFUNCTION()
		virtual void OnCollision() {}
	UFUNCTION()
		virtual void OffCollision() {}

public:
	UFUNCTION()
		virtual void StartLaunched();
	UFUNCTION()
		virtual void EndLaunched();

protected:
	UFUNCTION()
		virtual void OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	FProjectileBeginOverlap OnProjectileBeginOverlap;
	FProjectileEndLaunched OnProjectileEndLaunched;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class UProjectileMovementComponent* ProjectileMovement;

protected:
	EProjectileType ProjectileType = EProjectileType::E_Max;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
		bool bPenetrable = false;

private:
	UPROPERTY(VisibleAnywhere, Category = "Life Span")
		FTimerHandle OffVisibilityHandler;
	UPROPERTY(VisibleAnywhere, Category = "Life Span")
		FTimerHandle ExpireHandler;
	UPROPERTY(EditDefaultsOnly, Category = "LifeSpan")
		float OffVisibilityRate = 0.5f;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Life Span")
		float ExpireRate = 0.f;


protected:
	/*	**********************************************************
	* 
	*	[Remark_23-03-20]
	*	
	*	목적: Object Pooling 정보를 담은 구조체 정의
	*	
	*	설명: Object Pooling에 필요한 정보들을 해당 구조체에 저장합니다.
	* 
	*	**********************************************************/

	struct PoolObjectInfo
	{
	public:
		class AProjectile** Next = nullptr;
		uint8 Index = -1;
		bool IsActive = false;
		bool IsLoaded = false;
	}PoolObjectInfo_Struct;

public:
	FORCEINLINE void SetIsPenetrable(bool InBool) { bPenetrable = InBool; }
	FORCEINLINE bool GetIsPenetrable() { return bPenetrable; }

	FORCEINLINE void SetExpireRate(float InFloat) { ExpireRate = InFloat; }
	FORCEINLINE float GetExpireRate() { return ExpireRate; }

	FORCEINLINE void SetIndex(uint8 InIndex) { PoolObjectInfo_Struct.Index = InIndex; }
	FORCEINLINE uint8 GetIndex() { return PoolObjectInfo_Struct.Index; }

	FORCEINLINE void SetNext(class AProjectile** InProjectilePtr ) { PoolObjectInfo_Struct.Next = InProjectilePtr; }
	FORCEINLINE AProjectile** GetNext() { return PoolObjectInfo_Struct.Next; }

	FORCEINLINE void SetIsActive(bool InBool) { PoolObjectInfo_Struct.IsActive = InBool; }
	FORCEINLINE bool GetIsActive() { return PoolObjectInfo_Struct.IsActive; }

	FORCEINLINE void SetIsLoaded(bool InBool) { PoolObjectInfo_Struct.IsLoaded = InBool; }
	FORCEINLINE bool GetIsLoaded() { return PoolObjectInfo_Struct.IsLoaded; }

};
