// Fill out your copyright notice in the Description page of Project Settings.


#include "Tasks/BTT_LookAround_RuniskovsDan.h"

#include "Utils_RuniskovsDan.h"
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
	auto& Blackboard{ MyBTTUtils_RuniskovsDan::GetBlackboard(OwnerComp) };
	if (!Blackboard.GetValueAsBool(ShouldLookAroundKey.SelectedKeyName)) return EBTNodeResult::Failed;

	// --- Avoid repetition ---
	Blackboard.SetValueAsBool(ShouldLookAroundKey.SelectedKeyName, false);
	
	// --- Reset Yaw ---
	AccumulatedYaw = 0.f;
	
	return EBTNodeResult::InProgress;
}

void UBTT_LookAround_RuniskovsDan::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	const float Step{ DegPerSec * DeltaSeconds };

	AccumulatedYaw += Step;

	const auto CurrentYaw{ static_cast<float>(Survivalist->GetActorRotation().Yaw) };
	const auto NewYaw{ CurrentYaw + Step };

	Survivalist->SetActorRotation(FRotator{ 0.f, NewYaw, 0.f });

	// --- Full turn complete ---
	if (AccumulatedYaw >= 360.f)
	{
		// --- If inside the house -> mark it ---
		HouseTracker->MarkCurrentHouse();

		auto& BlackboardComponent{ MyBTTUtils_RuniskovsDan::GetBlackboard(OwnerComp) };
		BlackboardComponent.SetValueAsObject(TEXT("House"), nullptr);

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
