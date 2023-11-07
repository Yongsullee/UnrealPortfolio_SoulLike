#include "BTTaskNode_IsSkillValid.h"
#include "AIController/AIC_EnemyBase.h"
#include "GameObjects/GameObject.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTTaskNode_IsSkillValid::UBTTaskNode_IsSkillValid(const FObjectInitializer& InObjectInitializer)
{
    bCreateNodeInstance = true;
    NodeName = "IsSkillValid";
}

EBTNodeResult::Type UBTTaskNode_IsSkillValid::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

    AAIC_EnemyBase* AIController = Cast<AAIC_EnemyBase>(OwnerComp.GetAIOwner());
    UBlackboardComponent* BBComponent = OwnerComp.GetBlackboardComponent();
    AGameObject* OwningObject = Cast<AGameObject>(AIController->GetPawn());

    if (!AIController || !BBComponent || !OwningObject) return EBTNodeResult::Failed;

    EAICombatPhase Phase = static_cast<EAICombatPhase>(BBComponent->GetValueAsEnum("AICombatPhase"));
    if (Phase == EAICombatPhase::E_Max)
    {
        return EBTNodeResult::Failed;
    }

    uint8 AICombatPahseNum = static_cast<uint8>(Phase);
    int ValidSkillNum = OwningObject->GetValidSkillInAICombatPhase(AICombatPahseNum);

    if (ValidSkillNum == -1)
    {
        BBComponent->SetValueAsBool("IsSkillAttackValid", false);
        BBComponent->SetValueAsInt("ValidSkillNum", ValidSkillNum);

        return EBTNodeResult::Succeeded;
    }

    BBComponent->SetValueAsBool("IsSkillAttackValid", true);
    BBComponent->SetValueAsInt("ValidSkillNum", ValidSkillNum);

    return EBTNodeResult::Succeeded;

}
