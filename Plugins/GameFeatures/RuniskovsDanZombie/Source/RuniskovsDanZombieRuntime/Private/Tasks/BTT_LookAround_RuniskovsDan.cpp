// Fill out your copyright notice in the Description page of Project Settings.


#include "Tasks/BTT_LookAround_RuniskovsDan.h"

#include "Tasks/Utils_RuniskovsDan.h"
#include "Survivor/SurvivorPawn.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/HouseTrackerComponent_RuniskovsDan.h"
#include "Components/SteeringComponent_RuniskovsDan.h"

EBTNodeResult::Type UBTT_LookAround_RuniskovsDan::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Survivalist = MyBTTUtils_RuniskovsDan::GetSurvivorPawn(OwnerComp);
	
	// --- Getting the house tracker component ---
	HouseTracker = Survivalist->GetComponentByClass<UHouseTrackerComponent_RuniskovsDan>();
	check(HouseTracker);
	
	// --- Don't look around if shouldn't ---
	if (const auto& Blackboard{ MyBTTUtils_RuniskovsDan::GetBlackboard(OwnerComp) };
		!Blackboard.GetValueAsBool(ShouldLookAroundKey.SelectedKeyName)) return EBTNodeResult::Failed;
	
	// --- Reset Yaw ---
	AccumulatedYaw = 0.f;
	
	return EBTNodeResult::InProgress;
}

void UBTT_LookAround_RuniskovsDan::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	// --- Pretend to turn ---
	const float Step{ DegPerSec * DeltaSeconds };
	AccumulatedYaw += Step;
	const auto CurrentYaw{ static_cast<float>(Survivalist->GetActorRotation().Yaw) };
	const auto NewYaw{ CurrentYaw + Step };
	Survivalist->SetActorRotation(FRotator{ 0.f, NewYaw, 0.f });

	// --- Full turn complete ---
	if (AccumulatedYaw >= 360.f)
	{
		// --- Reset Yaw ---
		AccumulatedYaw = 0.f;
		
		auto& Blackboard = MyBTTUtils_RuniskovsDan::GetBlackboard(OwnerComp);
		Blackboard.SetValueAsBool(ShouldLookAroundKey.SelectedKeyName, false);
		Blackboard.SetValueAsObject(TEXT("House"), nullptr);
		
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
