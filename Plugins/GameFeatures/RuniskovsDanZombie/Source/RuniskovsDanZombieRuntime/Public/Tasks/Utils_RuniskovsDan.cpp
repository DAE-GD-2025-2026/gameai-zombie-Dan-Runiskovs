#include "Tasks/Utils_RuniskovsDan.h"

#include "BehaviorTree/BehaviorTreeComponent.h"

namespace MyBTTUtils_RuniskovsDan
{
	ASurvivorPawn* GetSurvivorPawn(const UBehaviorTreeComponent& OwnerComp) noexcept
	{
		auto* const controller = OwnerComp.GetAIOwner();
		check(controller);
		
		auto* const survivorPawn = Cast<ASurvivorPawn>(controller->GetPawn());
		check(survivorPawn);
		
		return survivorPawn;
	}
}
