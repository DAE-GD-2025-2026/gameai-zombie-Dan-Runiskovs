// Fill out your copyright notice in the Description page of Project Settings.


#include "Tasks/BTT_Flee_RuniskovsDan.h"
#include "Tasks/Utils_RuniskovsDan.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Survivor/SurvivorPawn.h"
#include "Components/SteeringComponent_RuniskovsDan.h"

EBTNodeResult::Type UBTT_Flee_RuniskovsDan::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Blackboard = OwnerComp.GetBlackboardComponent();
	Survivalist = MyBTTUtils_RuniskovsDan::GetSurvivorPawn(OwnerComp);
	
	TargetPosition = Blackboard->GetValueAsVector(TargetPoint.SelectedKeyName);
	
	SteeringComponent = Survivalist->GetComponentByClass<USteeringComponent_RuniskovsDan>();
	SteeringComponent->SetBehavior<FFlee_RuniskovsDan>();
	
	// --- Do the whole zombie check... maybe flee only regular zombies, others - fight ---
	// --- Not gonna happen ---
	SteeringComponent->SetTarget(TargetPosition);
	
	return EBTNodeResult::InProgress;
}

void UBTT_Flee_RuniskovsDan::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (constexpr float SafeDstSquared{ 1000.f }; 
		(Survivalist->GetActorLocation() - TargetPosition).SquaredLength() >= SafeDstSquared)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
