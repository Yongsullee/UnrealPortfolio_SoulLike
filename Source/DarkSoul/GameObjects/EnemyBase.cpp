#include "EnemyBase.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ActorComponents/C_HealthComponent.h"
#include "ActorComponents/C_StateComponent.h"
#include "HUD/HUD_EnemyProfile.h"
#include "AIController/AIC_EnemyBase.h"

AEnemyBase::AEnemyBase()
{

	PrimaryActorTick.bCanEverTick = false;

	HUDComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HUD"));

	CheckTrue(!(GetCharacterMovement()) || !HUDComponent);
	GetCharacterMovement()->MaxWalkSpeed = 400.f;

	HUDComponent->SetupAttachment(GetMesh());
	HUDComponent->SetRelativeScale3D(FVector(2.f, 2.f, 2.f));
	HUDComponent->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	HUDComponent->SetRelativeLocation(FVector(0.f, 0.f, 200.f));

	// Faction
	Faction = EFaction::E_Enemy;

}

void AEnemyBase::BeginPlay()
{
	
	Super::BeginPlay();


	// #1. Delegate
	if(HealthComponent)
		HealthComponent->OnUpdateEnemyHUDProfileHealthBar.BindUFunction(this, "UpdateHUDProfileHealthBar");

	AAIC_EnemyBase* EnemyAIController = Cast<AAIC_EnemyBase>(GetController());
	if (EnemyAIController)
	{
		EnemyAIController->OnAgentStartAttack.AddDynamic(this, &AGameObject::Attack);
		EnemyAIController->OnAgentRunAICombatPhase.AddDynamic(this, &AEnemyBase::RunAICombatPhase);
	}

}

void AEnemyBase::IsStartDamaged(AActor* InDamageCauser, float InImpulseAmount, bool bStatusEffectTriggered)
{
	
	if (GetStateComponent()->GetStateType() != EStateType::E_Attacking
		&& GetStateComponent()->GetStateType() != EStateType::E_Jumping
		&& GetStateComponent()->GetStateType() != EStateType::E_KnockDown
		&& GetStateComponent()->GetStateType() != EStateType::E_Skilling
		&& GetStateComponent()->GetStateType() != EStateType::E_WakeUp
		&& GetStateComponent()->GetStateType() != EStateType::E_Rolling
		&& GetStateComponent()->GetStateType() != EStateType::E_Dead
		&& InDamageCauser)
	{
		AActor* DamageCauserOwner = InDamageCauser->GetOwner();
		if (DamageCauserOwner)
		{
			ACharacter* DamageCauserOwnerCharacter = Cast<ACharacter>(DamageCauserOwner);
			if (DamageCauserOwnerCharacter && DamageCauserOwnerCharacter != this)
			{
				LookAtDamageCauser(&DamageCauserOwnerCharacter);
			}
		}
	}
	
	Super::IsStartDamaged(InDamageCauser, InImpulseAmount, bStatusEffectTriggered);

}

void AEnemyBase::UpdateHUDProfileHealthBar(float InFloat1, float InFloat2)
{

	UHUD_EnemyProfile* HUD_EnemyProfileWidget = Cast<UHUD_EnemyProfile>(HUDComponent->GetWidget());
	CheckNull(HUD_EnemyProfileWidget);

	HUD_EnemyProfileWidget->SetEnemyHealthBar(InFloat1, InFloat2);

}

void AEnemyBase::RunAICombatPhase(EAICombatPhase InEnum)
{
	Super::RunAICombatPhase(InEnum);
}
