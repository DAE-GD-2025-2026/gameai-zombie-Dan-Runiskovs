// Fill out your copyright notice in the Description page of Project Settings.


#include "Tasks/BTT_LookAround_RuniskovsDan.h"

#include "Utils_RuniskovsDan.h"
#include "Survivor/SurvivorPawn.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/HouseTrackerComponent_RuniskovsDan.h"
#include "Components/SteeringComponent_RuniskovsDan.h"

EBTNodeResult::Type UBTT_LookAround_RuniskovsDan::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UE_LOG(LogTemp, Warning, TEXT("LOOK AROUND STARTED"));
	
	Survivalist = MyBTTUtils_RuniskovsDan::GetSurvivorPawn(OwnerComp);
	
	// --- Getting the house tracker component ---
	HouseTracker = Survivalist->GetComponentByClass<UHouseTrackerComponent_RuniskovsDan>();
	check(HouseTracker);
	
	// --- Don't look around if shouldn't ---
	auto& Blackboard{ MyBTTUtils_RuniskovsDan::GetBlackboard(OwnerComp) };
	if (!Blackboard.GetValueAsBool(ShouldLookAroundKey.SelectedKeyName)) return EBTNodeResult::Failed;
	
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
	
	UE_LOG(LogTemp, Warning,
	TEXT("Yaw = %.2f"),
	AccumulatedYaw);

	// --- Full turn complete ---
	if (AccumulatedYaw >= 360.f)
	{
		auto& Blackboard = MyBTTUtils_RuniskovsDan::GetBlackboard(OwnerComp);

		Blackboard.SetValueAsBool(ShouldLookAroundKey.SelectedKeyName, false);
		
		// --- If inside the house -> mark it ---
		HouseTracker->MarkCurrentHouse();

		auto& BlackboardComponent{ MyBTTUtils_RuniskovsDan::GetBlackboard(OwnerComp) };
		BlackboardComponent.SetValueAsObject(TEXT("House"), nullptr);
		
		UE_LOG(LogTemp, Warning, TEXT("LOOK AROUND FINISHED"));
		
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
