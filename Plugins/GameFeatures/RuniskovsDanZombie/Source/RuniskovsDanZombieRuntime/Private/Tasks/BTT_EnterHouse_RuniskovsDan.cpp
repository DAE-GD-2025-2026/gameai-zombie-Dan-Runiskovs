// Fill out your copyright notice in the Description page of Project Settings.


#include "Tasks/BTT_EnterHouse_RuniskovsDan.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/HouseTrackerComponent_RuniskovsDan.h"
#include "Survivor/SurvivorPawn.h"
#include "Village/House/House.h"
#include "Tasks/Utils_RuniskovsDan.h"
#include "Components/SteeringComponent_RuniskovsDan.h"

EBTNodeResult::Type UBTT_EnterHouse_RuniskovsDan::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// --- Get Survivalist ---
	Survivalist = MyBTTUtils_RuniskovsDan::GetSurvivorPawn(OwnerComp);
	
	// --- Get visible house ---
	const auto* BlackboardComponent{ OwnerComp.GetBlackboardComponent() };
	verify(BlackboardComponent);
	House = Cast<AHouse>(BlackboardComponent->GetValueAsObject(HouseKey.SelectedKeyName));
	verify(House);
	
	// --- Skip if inside ---
	if (const FVector2D Location{Survivalist->GetActorLocation().X, Survivalist->GetActorLocation().Y}; 
		MyBTTUtils_RuniskovsDan::IsPointInHouse(Location, House->GetBounds())) return EBTNodeResult::Failed;
	
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
	
	// --- Save Path data ---
	auto* Memory{ reinterpret_cast<FNodeMemory*>(NodeMemory) };
	Memory->Path = Path;
	Memory->CurrentPointIdx = 1;
	
	// --- And finally ---
	return EBTNodeResult::InProgress;
}

void UBTT_EnterHouse_RuniskovsDan::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	// --- Retrieve the memory ---
	auto* Memory = reinterpret_cast<FNodeMemory*>(NodeMemory);
	
	if (Memory->CurrentPointIdx >= static_cast<uint32_t>(Memory->Path.Num()))
	{
		if (HouseTracker && House)  HouseTracker->SetHouseVisited(*House);

		auto& Blackboard = MyBTTUtils_RuniskovsDan::GetBlackboard(OwnerComp);
		Blackboard.SetValueAsObject(HouseKey.SelectedKeyName, nullptr);
		Blackboard.SetValueAsBool(ShouldLookAroundKey.SelectedKeyName, true);

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
	
	// --- Save Waypoint ---
	const auto& CurrentWaypoint{ Memory->Path[Memory->CurrentPointIdx] };
	
	// Advancing to the next waypoint if the current one is reached
	if (const auto KindaThereSq{ KindaThereRadius * KindaThereRadius }; 
		(CurrentWaypoint - Survivalist->GetActorLocation()).SizeSquared() <= KindaThereSq)
	{		
		// --- Advanced enough ? Go next ---
		++Memory->CurrentPointIdx;
	}
	else
	{
		const auto* SteeringComp{Survivalist->GetComponentByClass<USteeringComponent_RuniskovsDan>()};
		verify(SteeringComp);
		
		SteeringComp->SetTarget(CurrentWaypoint);
	}
}

EBTNodeResult::Type UBTT_EnterHouse_RuniskovsDan::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Survivalist->GetMovementComponent()->StopMovementImmediately();
	return EBTNodeResult::Aborted;
}

void UBTT_EnterHouse_RuniskovsDan::SaveHouseAsVisited() const noexcept
{
	HouseTracker->SetHouseVisited(*House);
}
