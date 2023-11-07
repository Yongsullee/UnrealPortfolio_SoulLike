#include "DemonFist.h"
#include "Global/Global.h"

#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "ActorComponents/C_ProjectileSpawnerComponent.h"

#include "GameObjects/GameObject.h"


ADemonFist::ADemonFist()
{

	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USceneComponent>(this, &Root, TEXT("Root"));
	CHelpers::CreateComponent<USphereComponent>(this, &LeftHand, TEXT("Left Hand"), Root);
	CHelpers::CreateComponent<USphereComponent>(this, &RightHand, TEXT("Right Hand"), Root);
	CHelpers::CreateComponent<USphereComponent>(this, &Wing, TEXT("Wing"), Root);
	CHelpers::CreateComponent<USphereComponent>(this, &Mouth, TEXT("Mouth"), Root);
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &ZoneEffect, TEXT("Zone Effect"), Root);
	ProjectileSpawner = CreateDefaultSubobject<UC_ProjectileSpawnerComponent>(TEXT("Projectile Spawner"));

	SphereComponents.Reserve(SphereComponentsSize);
	SphereComponents.Add(LeftHand);
	SphereComponents.Add(RightHand);
	SphereComponents.Add(Wing);
	SphereComponents.Add(Mouth);

	for (auto sphere : SphereComponents)
	{
		sphere->SetSphereRadius(100.f);
		sphere->SetVisibility(true);
	}

	Wing->SetSphereRadius(150.f);
	Mouth->SetSphereRadius(32.f);

	// Particle System
	UParticleSystem* particle;
	CHelpers::GetAsset<UParticleSystem>(&particle, "ParticleSystem'/Game/AuraFX01/Particles/P_ky_aura26'");
	ZoneEffect->SetTemplate(particle);
	ZoneEffect->SetRelativeScale3D(FVector(3.f, 3.f, 1.5f));
	ZoneEffect->bAutoActivate = false;

	// Weapon Type
	WeaponType = EWeaponType::E_DemonFist;

	// Projectile Type
	ProjectileType = EProjectileType::E_Ray;

}

void ADemonFist::BeginPlay()
{
	
	Super::BeginPlay();

	// #0. Sphere Collision
	for (const auto sphere : SphereComponents)
	{
		sphere->OnComponentBeginOverlap.AddDynamic(this, &ADemonFist::OnCapsuleBeginOverlap);
	}

	// #1. Projectile Collision
	TArray<EProjectileType> TmpArray;
	ProjectileSpawner->GetProjectilePool().GenerateKeyArray(TmpArray);

	for (auto& Type : TmpArray)
	{
		ParallelFor(ProjectileSpawner->GetProjectilePool()[Type].Num(), [&](uint8 Idx) {
			ProjectileSpawner->GetProjectilePool()[Type][Idx]->OnProjectileBeginOverlap.BindUFunction(this, "MagicProjectileBeginOverlap");
			});
	}

}

void ADemonFist::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);

	if (bOnMagneticField) CreateMagneticField();

}

void ADemonFist::AttachComponentToOwnerComponent(USkeletalMeshComponent* InSkeletalMeshComponent)
{

	Super::AttachComponentToOwnerComponent(InSkeletalMeshComponent);

	LeftHand->AttachToComponent(InSkeletalMeshComponent, FAttachmentTransformRules::KeepRelativeTransform, TEXT("DemonFist_LeftHand"));
	RightHand->AttachToComponent(InSkeletalMeshComponent, FAttachmentTransformRules::KeepRelativeTransform, TEXT("DemonFist_RightHand"));
	Wing->AttachToComponent(InSkeletalMeshComponent, FAttachmentTransformRules::KeepRelativeTransform, TEXT("DemonFist_Wing"));
	Mouth->AttachToComponent(InSkeletalMeshComponent, FAttachmentTransformRules::KeepRelativeTransform, TEXT("DemonFist_Mouth"));
	ZoneEffect->AttachToComponent(InSkeletalMeshComponent, FAttachmentTransformRules::KeepRelativeTransform, TEXT("ZoneEffect"));


}

void ADemonFist::OnCollision()
{

	Super::OnCollision();

	for (const auto sphere : SphereComponents)
	{
		sphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}

}

void ADemonFist::OffCollision()
{

	Super::OffCollision();

	for (const auto sphere : SphereComponents)
	{
		sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

}

void ADemonFist::AttackSkill(const int InSkillNum)
{

	Super::AttackSkill(InSkillNum);

	FTimerHandle TimerHandler;
	FTimerDelegate TimerDelegate;

	switch (InSkillNum)
	{
	case 0:
		bOnMagneticField = true;
		break;
	case 1:
		TimerDelegate = FTimerDelegate::CreateUObject(this, &ADemonFist::Dash);
		GetWorldTimerManager().SetTimer(TimerHandler, TimerDelegate, 1.f, false);
	default:
		break;
	}

}

void ADemonFist::EndAttackSkill()
{

	Super::EndAttack();

	if (bOnMagneticField == true)
	{
		bOnMagneticField = false;
		OffCollision();
	}

}

void ADemonFist::CreateMagneticField()
{

	TArray<TEnumAsByte<EObjectTypeQuery>> NearbyActors;
	TEnumAsByte<EObjectTypeQuery> PawnObjectType = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn);
	NearbyActors.Add(PawnObjectType);

	AActor* OwnerActor = GetOwner();
	CheckNull(OwnerActor);

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(OwnerActor);

	TArray<FHitResult> HitResults;

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

	if (Result == true)
	{
		for (const auto& Hit : HitResults)
		{
			AGameObject* HitObject = Cast<AGameObject>(Hit.GetActor());
			AGameObject* OwnerObject = Cast<AGameObject>(OwnerActor);

			if ((HitObject != nullptr) 
				&& (OwnerObject != nullptr) 
				&& (OwnerObject->IsHostile(HitObject) == true)
				&& (AttackHitActors.Contains(HitObject) != true))
			{
				OnWeaponCollisionBeginOverlap.Execute(OwnerActor, HitObject, IsSkillAttack, SkillBasePower);
				AttackHitActors.Add(HitObject);
			}
		}
	}

}

void ADemonFist::Dash()
{

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());

	CheckTrue(!OwnerCharacter);

	FVector TargetLocation = OwnerCharacter->GetActorLocation() + (OwnerCharacter->GetActorForwardVector() * DashDistance);
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;

	UKismetSystemLibrary::MoveComponentTo(
		OwnerCharacter->GetRootComponent(),
		TargetLocation,
		OwnerCharacter->GetActorRotation(),
		false,
		false,
		0.15f,
		true,
		EMoveComponentAction::Move,
		LatentInfo
	);

}


void ADemonFist::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	AActor* OwnerActor = GetOwner();
	AGameObject* OwnerObject = Cast<AGameObject>(OwnerActor);
	AGameObject* OtherObject = Cast<AGameObject>(OtherActor);
	CheckTrue(!OtherActor || !OwnerObject || !OwnerActor || !OtherObject 
		|| OtherActor == OwnerActor || !OtherObject->IsHostile(OwnerObject));

	if (AttackHitActors.Contains(OtherActor) != true)
	{
		if (OnWeaponCollisionBeginOverlap.IsBound())
		{
			OnWeaponCollisionBeginOverlap.Execute(OwnerActor, OtherActor, IsSkillAttack, SkillBasePower);

			AttackHitActors.Add(OtherActor);
		}
	}

}

void ADemonFist::MagicProjectileBeginOverlap(AActor* OtherActor)
{

	AGameObject* OtherObject = Cast<AGameObject>(OtherActor);
	CheckTrue(!OtherObject);

	if (AttackHitActors.Contains(OtherActor) != true)
	{
		if (OnWeaponCollisionBeginOverlap.IsBound())
		{
			OnWeaponCollisionBeginOverlap.Execute(GetOwner(), OtherActor, IsSkillAttack, SkillBasePower);

			AttackHitActors.Add(OtherActor);
		}
	}

}
