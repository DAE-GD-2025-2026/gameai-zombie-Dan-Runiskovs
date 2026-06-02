#pragma once
#include "AIController.h"
#include "Survivor/SurvivorPawn.h"

namespace MyBTTUtils_RuniskovsDan
{
	ASurvivorPawn* GetSurvivorPawn(UBehaviorTreeComponent& OwnerComp) noexcept
	{
		auto* const controller = OwnerComp.GetAIOwner();
		check(controller);
		
		auto* const survivorPawn = Cast<ASurvivorPawn>(controller->GetPawn());
		check(survivorPawn);
		
		return survivorPawn;
	}
}
