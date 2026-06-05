// Fill out your copyright notice in the Description page of Project Settings.


#include "Tasks/BTT_EnterHouse_RuniskovsDan.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/HouseTrackerComponent_RuniskovsDan.h"
#include "Survivor/SurvivorPawn.h"
#include "Village/House/House.h"
#include "Utils_RuniskovsDan.h"
#include "Components/SteeringComponent_RuniskovsDan.h"

// Shoutout to Alex (LV) for helping me with this one
struct FNodeMemory final
{
	TArray<FVector> Path;
	uint32_t CurrentPointIdx{};
};

EBTNodeResult::Type UBTT_EnterHouse_RuniskovsDan::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// --- Get Survivalist ---
	Survivalist = MyBTTUtils_RuniskovsDan::GetSurvivorPawn(OwnerComp);
	
	// --- Get visible house ---
	auto* BlackboardComponent{ OwnerComp.GetBlackboardComponent() };
	verify(BlackboardComponent);
	House = Cast<AHouse>(BlackboardComponent->GetValueAsObject(HouseKey.SelectedKeyName));
	verify(House);
	
	// --- Skip if inside ---
	const auto Location{Survivalist->GetActorLocation().X, Survivalist->GetActorLocation().Y};
	if (MyBTTUtils_RuniskovsDan::IsPointInHouse(Location, House->GetBounds())) return EBTNodeResult::Failed;
	
	// --- Skip House if visited ---
	HouseTracker = Survivalist->GetComponentByClass<UHouseTrackerComponent_RuniskovsDan>();
	verify(HouseTracker);
	if (HouseTracker->IsHouseVisited(*House)) return EBTNodeResult::Failed;
	
	// --- Look Around when in ---
	auto& Blackboard{ MyBTTUtils_RuniskovsDan::GetBlackboard(OwnerComp) };
	Blackboard.SetValueAsBool(ShouldLookAroundKey.SelectedKeyName, true);
	
	// --- Go in the house ---
	auto* SteeringComponent{ Survivalist->GetComponentByClass<USteeringComponent_RuniskovsDan>() };
	verify(SteeringComponent);
	SteeringComponent->SetBehavior<FSeek_RuniskovsDan>();
	const auto HouseCenter{ House->GetBounds().Origin };
	
	// --- Calculate path ---
	const auto Path{ Survivalist->CalculatePath(HouseCenter) };
	
	// --- Save Path data ---
	auto* Memory{ reinterpret_cast<FNodeMemory*>(NodeMemory) };
	Memory->Path = Path;
	Memory->CurrentPointIdx = 1;
	
	// --- And finally ---
	return EBTNodeResult::InProgress;
}

void UBTT_EnterHouse_RuniskovsDan::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	// Finishing if the entire path was consumed
	auto* Memory = reinterpret_cast<FNodeMemory*>(NodeMemory);
	if (Memory->CurrentPointIdx >= static_cast<uint32_t>(Memory->Path.Num()))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
	// Saving the current waypoint data
	const auto& CurrentWaypoint{ Memory->Path[Memory->CurrentPointIdx] };

	// Advancing to the next waypoint if the current one is reached
	constexpr auto KindaThereSq{ 400.f }; // 20*20
	if ((CurrentWaypoint - Survivalist->GetActorLocation()).SizeSquared() <= KindaThereSq)
	{
		++Memory->CurrentPointIdx;
	}
	else
	{
		auto* SteeringBehaviorComponent{Survivalist->GetComponentByClass<USteeringComponent_RuniskovsDan>()};
		verify(SteeringBehaviorComponent);
		
		SteeringBehaviorComponent->SetTarget(CurrentWaypoint);
	}
}

EBTNodeResult::Type UBTT_EnterHouse_RuniskovsDan::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Survivalist->GetMovementComponent()->StopMovementImmediately();
	return EBTNodeResult::Aborted;
}

uint16 UBTT_EnterHouse_RuniskovsDan::GetInstanceMemorySize() const
{
	// Shoutout Alex for saving my headaches
	return sizeof(FNodeMemory);
}

void UBTT_EnterHouse_RuniskovsDan::SaveHouseAsVisited() const noexcept
{
	HouseTracker->SetHouseVisited(*House);
}
