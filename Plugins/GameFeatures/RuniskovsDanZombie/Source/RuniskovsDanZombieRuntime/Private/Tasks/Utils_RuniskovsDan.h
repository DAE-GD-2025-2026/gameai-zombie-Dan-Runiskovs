#pragma once
#include "AIController.h"
#include "Steering/FSteeringBehaviorBase_RuniskovsDan.h"


class USteeringComponent_RuniskovsDan;
class UBehaviorTreeComponent;
class ASurvivorPawn;
class AHouse;

namespace MyBTTUtils_RuniskovsDan
{
	// --- Survivor Pawn Getter
	ASurvivorPawn* GetSurvivorPawn(const UBehaviorTreeComponent& OwnerComp) noexcept;
	
	// --- Position Retriever --- 
	FVector2D GetSurvivalistLocation(const USteeringComponent_RuniskovsDan& SteeringComp) noexcept;
	
	// --- House box getter ---
	FBox GetHouseBounds(AHouse* House) noexcept;
}
