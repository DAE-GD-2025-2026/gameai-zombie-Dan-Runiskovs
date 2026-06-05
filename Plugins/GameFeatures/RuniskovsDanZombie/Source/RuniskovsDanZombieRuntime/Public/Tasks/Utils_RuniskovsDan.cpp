#include "Tasks/Utils_RuniskovsDan.h"
#include "Survivor/SurvivorPawn.h"
#include "Village/House/House.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Components/SteeringComponent_RuniskovsDan.h"
#include "BehaviorTree/BlackboardComponent.h"

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
		auto* Survivalist{ SteeringComp.GetOwner() };
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

	bool IsPointInHouse(FVector2D Location, const FHouseBounds& Bounds) noexcept
	{
		return	FMath::Abs(Location.X - Bounds.Origin.X) <= Bounds.Extent.X &&
				FMath::Abs(Location.Y - Bounds.Origin.Y) <= Bounds.Extent.Y;
	}

	UBlackboardComponent& GetBlackboard(UBehaviorTreeComponent& OwnerComp) noexcept
	{
		auto* BlackboardComponent{ OwnerComp.GetBlackboardComponent() };
		verify(BlackboardComponent);
		return *BlackboardComponent;
	}
}
