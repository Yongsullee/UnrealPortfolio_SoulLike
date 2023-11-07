#include "Ghost_Axe.h"
#include "Global/Global.h"

#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "ActorComponents/C_ProjectileSpawnerComponent.h"

#include "GameObjects/GameObject.h"

AGhost_Axe::AGhost_Axe()
{

	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USceneComponent>(this, &Root, TEXT("Root"));

	CHelpers::CreateComponent<USphereComponent>(this, &Axe, TEXT("Axe"), Root);

	CHelpers::CreateComponent<USphereComponent>(this, &LeftHand_Cast_01, TEXT("Left Hand Cast 01"), Root);

	CHelpers::CreateComponent<USphereComponent>(this, &LeftHand_Cast_02, TEXT("Left Hand Cast 02"), Root);
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &LeftHand_Cast_02_SkillEffect, TEXT("LeftHand Cast 02 Skill Effect"), LeftHand_Cast_02);

	CHelpers::CreateComponent<USphereComponent>(this, &RightHand_Cast_01, TEXT("Right Hand Cast 01"), Root);
	CHelpers::CreateComponent<UParticleSystemComponent>(this, &RightHand_Cast_01_SkillEffect, TEXT("Right Hand Cast 01 SKill Effect"), RightHand_Cast_01);

	CHelpers::CreateComponent<USphereComponent>(this, &RightHand_Cast_02, TEXT("Right Hand Cast 02"), Root);

	CHelpers::CreateComponent<UParticleSystemComponent>(this, &ZoneEffect, TEXT("Zone Effect"), Root);

	ProjectileSpawner = CreateDefaultSubobject<UC_ProjectileSpawnerComponent>(TEXT("Projectile Spawner"));

	SphereComponents.Reserve(SphereComponentsSize);
	SphereComponents.Add(RightHand_Cast_01);
	SphereComponents.Add(RightHand_Cast_02);
	SphereComponents.Add(LeftHand_Cast_01);
	SphereComponents.Add(LeftHand_Cast_02);
	SphereComponents.Add(Axe);

	for (const auto sphere : SphereComponents)
	{
		sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		sphere->SetVisibility(false);
	}

	RightHand_Cast_01->SetSphereRadius(32.f);
	RightHand_Cast_02->SetSphereRadius(32.f);
	LeftHand_Cast_01->SetSphereRadius(32.f);
	LeftHand_Cast_02->SetSphereRadius(32.f);
	Axe->SetSphereRadius(150.f);

	// Particle System
	UParticleSystem* particle;
	
	// #1. Zone Effect
	CHelpers::GetAsset<UParticleSystem>(&particle, "ParticleSystem'/Game/ParagonSevarog/FX/Particles/P_Sevarog_LevelStart'");
	ZoneEffect->SetTemplate(particle);
	ZoneEffect->SetRelativeScale3D(FVector(3.f, 3.f, 1.5f));
	ZoneEffect->bAutoActivate = false;

	// #2. Skill01 Effect
	CHelpers::GetAsset<UParticleSystem>(&particle, "ParticleSystem'/Game/ParagonSevarog/FX/Particles/Abilities/SoulSiphon/FX/P_GhostHand'");
	LeftHand_Cast_02_SkillEffect->SetTemplate(particle);
	LeftHand_Cast_02_SkillEffect->SetRelativeScale3D(FVector(3.f, 3.f, 1.5f));
	LeftHand_Cast_02_SkillEffect->bAutoActivate = false;

	// #3. Skill02 Effects
	CHelpers::GetAsset<UParticleSystem>(&particle, "ParticleSystem'/Game/ParagonSevarog/FX/Particles/Abilities/Ultimate/FX/P_UltimateLooping'");
	RightHand_Cast_01_SkillEffect->SetTemplate(particle);
	RightHand_Cast_01_SkillEffect->SetRelativeScale3D(FVector(3.f, 3.f, 1.5f));
	RightHand_Cast_01_SkillEffect->bAutoActivate = false;

	// Weapon Type
	WeaponType = EWeaponType::E_Ghost_Axe;

	// Projectile Type
	ProjectileType = EProjectileType::E_Ray;

}

void AGhost_Axe::BeginPlay()
{

	Super::BeginPlay();

	// #0. Sphere Collision
	for (const auto sphere : SphereComponents)
	{
		sphere->OnComponentBeginOverlap.AddDynamic(this, &AGhost_Axe::OnCapsuleBeginOverlap);
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

void AGhost_Axe::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);

}

void AGhost_Axe::AttachComponentToOwnerComponent(USkeletalMeshComponent* InSkeletalMeshComponent)
{

	Super::AttachComponentToOwnerComponent(InSkeletalMeshComponent);

	if (Axe && InSkeletalMeshComponent->DoesSocketExist(TEXT("Weapon_02")))
		Axe->AttachToComponent(InSkeletalMeshComponent, FAttachmentTransformRules::KeepRelativeTransform, TEXT("Weapon_01"));

	if (RightHand_Cast_01 && InSkeletalMeshComponent->DoesSocketExist(TEXT("hand_r_cast_01")))
		RightHand_Cast_01->AttachToComponent(InSkeletalMeshComponent, FAttachmentTransformRules::KeepRelativeTransform, TEXT("hand_r_cast_01"));

	if (RightHand_Cast_02 && InSkeletalMeshComponent->DoesSocketExist(TEXT("hand_r_cast_02")))
		RightHand_Cast_02->AttachToComponent(InSkeletalMeshComponent, FAttachmentTransformRules::KeepRelativeTransform, TEXT("hand_r_cast_02"));

	if (LeftHand_Cast_01 && InSkeletalMeshComponent->DoesSocketExist(TEXT("hand_l_cast_01")))
		LeftHand_Cast_01->AttachToComponent(InSkeletalMeshComponent, FAttachmentTransformRules::KeepRelativeTransform, TEXT("hand_l_cast_01"));

	if (LeftHand_Cast_02 && InSkeletalMeshComponent->DoesSocketExist(TEXT("hand_l_cast_02")))
		LeftHand_Cast_02->AttachToComponent(InSkeletalMeshComponent, FAttachmentTransformRules::KeepRelativeTransform, TEXT("hand_l_cast_02"));

	if (ZoneEffect && InSkeletalMeshComponent->DoesSocketExist(TEXT("ZoneEffect")))
		ZoneEffect->AttachToComponent(InSkeletalMeshComponent, FAttachmentTransformRules::KeepRelativeTransform, TEXT("ZoneEffect"));

}

void AGhost_Axe::OnCollision()
{

	Super::OnCollision();

	for (const auto sphere : SphereComponents)
	{
		sphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}

}

void AGhost_Axe::OffCollision()
{

	Super::OffCollision();

	for (const auto sphere : SphereComponents)
	{
		sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

}

void AGhost_Axe::AttackSkill(const int InSkillNum)
{

	Super::AttackSkill(InSkillNum);

	FTimerHandle TimerHandler;
	FTimerDelegate TimerDelegate;

	switch (InSkillNum)
	{
	case 0:
		GetWorld()->GetTimerManager().SetTimer(TimerHandler, FTimerDelegate::CreateLambda([&]() {
			CreateMagneticField();
			}), 1.5f, false);
		break;
	case 1:
		LeftHand_Cast_02_SkillEffect->Activate();

		TimerDelegate = FTimerDelegate::CreateUObject(this, &AGhost_Axe::Dash);
		GetWorldTimerManager().SetTimer(TimerHandler, TimerDelegate, 0.2f, false);
	default:
		break;
	}

}

void AGhost_Axe::EndAttackSkill()
{

	Super::EndAttackSkill();

	OffCollision();

}

void AGhost_Axe::CreateMagneticField()
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
		OwnerActor->GetActorLocation() + 50.f,
		EnemyDetectionMaxRadius,
		NearbyActors,
		false,
		IgnoreActors,
		EDrawDebugTrace::None,
		HitResults,
		true,
		FLinearColor::Transparent,
		FLinearColor::Transparent,
		5.f
	);

	if (Result == true)
	{
		for (const auto& Hit : HitResults)
		{
			AGameObject* HitObject = Cast<AGameObject>(Hit.GetActor());
			AGameObject* OwnerObject = Cast<AGameObject>(OwnerActor);

			if (HitObject && OwnerObject && OwnerObject->IsHostile(HitObject) && !AttackHitActors.Contains(HitObject))
			{
				OnWeaponCollisionBeginOverlap.Execute(OwnerActor, HitObject, IsSkillAttack, SkillBasePower);	

				AttackHitActors.Add(HitObject);
			}
		}
	}

}


void AGhost_Axe::Dash()
{

	AActor* OwnerActor = GetOwner();
	if (OwnerActor)
	{
		ACharacter* OwnerCharacter = Cast<ACharacter>(OwnerActor);
		if (OwnerCharacter)
		{
			FVector TargetLocation = OwnerCharacter->GetActorLocation() + (OwnerCharacter->GetActorForwardVector() * DashDistance);
			FLatentActionInfo LatentInfo;
			LatentInfo.CallbackTarget = this;

			UKismetSystemLibrary::MoveComponentTo(
				OwnerCharacter->GetRootComponent(),
				TargetLocation,
				OwnerCharacter->GetActorRotation(),
				true,
				true,
				0.2f,
				true,
				EMoveComponentAction::Move,
				LatentInfo
			);

		}
	}

}

void AGhost_Axe::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	AActor* OwnerActor = GetOwner();
	AGameObject* OwnerObject = Cast<AGameObject>(OwnerActor);
	AGameObject* OtherObject = Cast<AGameObject>(OtherActor);

	CheckTrue(!OtherActor || !OwnerObject || !OwnerActor || !OtherObject || OtherActor == OwnerActor || !OtherObject->IsHostile(OwnerObject));

	if (!AttackHitActors.Contains(OtherActor))
	{
		if (OnWeaponCollisionBeginOverlap.IsBound())
		{
			OnWeaponCollisionBeginOverlap.Execute(OwnerActor, OtherActor, IsSkillAttack, SkillBasePower);

			AttackHitActors.Add(OtherActor);
		}
	}

}

void AGhost_Axe::MagicProjectileBeginOverlap(AActor* OtherActor)
{

	AGameObject* DamagedObject = Cast<AGameObject>(OtherActor);
	CheckNull(DamagedObject);

	if (!AttackHitActors.Contains(OtherActor))
	{
		if (OnWeaponCollisionBeginOverlap.IsBound())
		{
			OnWeaponCollisionBeginOverlap.Execute(GetOwner(), OtherActor, IsSkillAttack, SkillBasePower);

			AttackHitActors.Add(OtherActor);
		}
	}

}
