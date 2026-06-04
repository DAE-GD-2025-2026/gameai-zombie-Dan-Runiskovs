// Fill out your copyright notice in the Description page of Project Settings.


#include "Tasks/BTT_Flee_RuniskovsDan.h"
#include "Utils_RuniskovsDan.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Steering/SteeringComponent_RuniskovsDan.h"

EBTNodeResult::Type UBTT_Flee_RuniskovsDan::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	m_Blackboard = OwnerComp.GetBlackboardComponent();
	m_pSurvivor = MyBTTUtils_RuniskovsDan::GetSurvivorPawn(OwnerComp);
	
	m_TargetPos = m_Blackboard->GetValueAsVector(m_TargetPoint.SelectedKeyName);
	
	m_pSteeringComponent = m_pSurvivor->GetComponentByClass<USteeringComponent_RuniskovsDan>();
	m_pSteeringComponent->SetBehavior<FFlee_RuniskovsDan>();
	
	// Do the whole zombie check... maybe flee only regular zombies, others - fight
	m_pSteeringComponent->SetTarget(m_TargetPos);
	
	return EBTNodeResult::InProgress;
}

void UBTT_Flee_RuniskovsDan::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	constexpr float SafeDstSquared = 1000.f;
	
	if ((m_pSurvivor->GetActorLocation() - m_TargetPos).SquaredLength() >= SafeDstSquared)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
