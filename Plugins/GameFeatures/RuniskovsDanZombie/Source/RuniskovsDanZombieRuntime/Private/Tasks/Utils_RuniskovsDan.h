#pragma once
#include "AIController.h"
#include "Survivor/SurvivorPawn.h"

class UBehaviorTreeComponent;

namespace MyBTTUtils_RuniskovsDan
{
	ASurvivorPawn* GetSurvivorPawn(const UBehaviorTreeComponent& OwnerComp) noexcept;
	
	
}
