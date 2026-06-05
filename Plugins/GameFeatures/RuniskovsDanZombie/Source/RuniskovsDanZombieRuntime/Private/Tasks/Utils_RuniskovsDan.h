#pragma once
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Steering/FSteeringBehaviorBase_RuniskovsDan.h"
#include "Village/House/House.h"


class USteeringComponent_RuniskovsDan;
class ASurvivorPawn;
class AHouse;
class ABaseItem;

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
	template <typename ObjectType>
	ObjectType* GetBlackboardObject(UBehaviorTreeComponent& OwnerComp, const FName& BlackboardKeyName) noexcept
	{
		const auto* BlackboardComponent{ OwnerComp.GetBlackboardComponent() };
		verify(BlackboardComponent);
		
		auto* Object{ Cast<ObjectType>(BlackboardComponent->GetValueAsObject(BlackboardKeyName)) };
		
		return Object;
	}
	
	// --- Inventory ---
	TArray<ABaseItem*> GetInventory(const ASurvivorPawn& Survivalist) noexcept;
}
