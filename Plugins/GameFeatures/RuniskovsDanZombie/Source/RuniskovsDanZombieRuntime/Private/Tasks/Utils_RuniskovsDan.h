#pragma once
#include "AIController.h"
#include "Steering/FSteeringBehaviorBase_RuniskovsDan.h"
#include "Village/House/House.h"


class USteeringComponent_RuniskovsDan;
class UBehaviorTreeComponent;
class UBlackboardComponent;
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
	bool IsPointInHouse(FVector2D Location, const FHouseBounds& Bounds) noexcept;
	
	// --- Blackboard ---
	UBlackboardComponent& GetBlackboard(UBehaviorTreeComponent& OwnerComp) noexcept;
}
