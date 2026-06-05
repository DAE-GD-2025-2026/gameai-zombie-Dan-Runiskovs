#include "Tasks/Utils_RuniskovsDan.h"
#include "Survivor/SurvivorPawn.h"
#include "Village/House/House.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Components/SteeringComponent_RuniskovsDan.h"

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

	FVector2D GetSurvivalistLocation(const USteeringComponent_RuniskovsDan& SteeringComp) noexcept
	{
		AActor* Survivalist{ SteeringComp.GetOwner() };
		verify(Survivalist);
		const FVector2D Position{ Survivalist->GetActorLocation().X, Survivalist->GetActorLocation().Y };
		return Position;
	}

	FBox GetHouseBounds(AHouse* House) noexcept
	{
		// --- House comes in here Pre-verified ---
		FVector HouseOrigin, HouseExtent;
		House->GetActorBounds(true, HouseOrigin, HouseExtent);
		
		// --- Compute bounds ---
		const FBox HouseBounds(HouseOrigin - HouseExtent, HouseOrigin + HouseExtent);
		return HouseBounds;
	}
}
