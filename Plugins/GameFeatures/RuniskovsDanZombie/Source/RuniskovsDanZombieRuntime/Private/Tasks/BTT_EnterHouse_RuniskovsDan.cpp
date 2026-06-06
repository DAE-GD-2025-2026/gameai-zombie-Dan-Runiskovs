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
	UE_LOG(LogTemp, Warning, TEXT("ENTER HOUSE TASK STARTED"));
	// --- Get Survivalist ---
	Survivalist = MyBTTUtils_RuniskovsDan::GetSurvivorPawn(OwnerComp);
	
	// --- Get visible house ---
	auto* BlackboardComponent{ OwnerComp.GetBlackboardComponent() };
	verify(BlackboardComponent);
	House = Cast<AHouse>(BlackboardComponent->GetValueAsObject(HouseKey.SelectedKeyName));
	verify(House);
	
	// --- Skip if inside ---
	const FVector2D Location{Survivalist->GetActorLocation().X, Survivalist->GetActorLocation().Y};
	if (MyBTTUtils_RuniskovsDan::IsPointInHouse(Location, House->GetBounds())) return EBTNodeResult::Failed;
	
	// --- Skip House if visited ---
	HouseTracker = Survivalist->GetComponentByClass<UHouseTrackerComponent_RuniskovsDan>();
	verify(HouseTracker);
	if (HouseTracker->IsHouseVisited(*House)) return EBTNodeResult::Failed;
	
	// --- Go in the house ---
	auto* SteeringComponent{ Survivalist->GetComponentByClass<USteeringComponent_RuniskovsDan>() };
	verify(SteeringComponent);
	SteeringComponent->SetBehavior<FSeek_RuniskovsDan>();
	const auto HouseCenter{ House->GetBounds().Origin };
	
	// --- Calculate path ---
	const auto Path{ Survivalist->CalculatePath(HouseCenter) };
	
	UE_LOG(LogTemp, Warning,
		TEXT("ENTER HOUSE: Path Size = %d"),
		Path.Num());

	for (int32 i{}; i < Path.Num(); ++i)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("Waypoint %d : %s"),
			i,
			*Path[i].ToString());
	}
	
	// --- Save Path data ---
	auto* Memory{ reinterpret_cast<FNodeMemory*>(NodeMemory) };
	Memory->Path = Path;
	Memory->CurrentPointIdx = 1;
	
	// --- And finally ---
	return EBTNodeResult::InProgress;
}

void UBTT_EnterHouse_RuniskovsDan::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UE_LOG(LogTemp, Warning, TEXT("ENTER HOUSE TICK"));
	
	// Finishing if the entire path was consumed
	auto* Memory = reinterpret_cast<FNodeMemory*>(NodeMemory);
	UE_LOG(LogTemp, Warning,
		TEXT("CurrentIdx = %d / %d"),
		Memory->CurrentPointIdx,
		Memory->Path.Num());
	
	if (Memory->CurrentPointIdx >= static_cast<uint32_t>(Memory->Path.Num()))
	{
		UE_LOG(LogTemp, Warning, TEXT("ENTER HOUSE SUCCEEDED"));
		
		auto& Blackboard = MyBTTUtils_RuniskovsDan::GetBlackboard(OwnerComp);

		Blackboard.SetValueAsBool(ShouldLookAroundKey.SelectedKeyName,true);
		
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
	// Saving the current waypoint data
	const auto& CurrentWaypoint{ Memory->Path[Memory->CurrentPointIdx] };
	const float Distance =
		(CurrentWaypoint - Survivalist->GetActorLocation()).Size();

	UE_LOG(LogTemp, Warning,
		TEXT("Distance To Waypoint = %.2f"),
		Distance);
	
	// Advancing to the next waypoint if the current one is reached
	constexpr auto KindaThereSq{ 35.f * 35.f }; // 20*20
	if ((CurrentWaypoint - Survivalist->GetActorLocation()).SizeSquared() <= KindaThereSq)
	{
		UE_LOG(LogTemp, Warning,
		TEXT("Reached Waypoint %d"),
		Memory->CurrentPointIdx);
		
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
	UE_LOG(LogTemp, Error, TEXT("ENTER HOUSE ABORTED"));
	
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
