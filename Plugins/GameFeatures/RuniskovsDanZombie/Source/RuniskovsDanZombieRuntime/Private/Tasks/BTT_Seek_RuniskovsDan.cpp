// Fill out your copyright notice in the Description page of Project Settings.


#include "Tasks/BTT_Seek_RuniskovsDan.h"

#include "Utils_RuniskovsDan.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Survivor/SurvivorPawn.h"
#include "Components/SteeringComponent_RuniskovsDan.h"


EBTNodeResult::Type UBTT_Seek_RuniskovsDan::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
 	m_Blackboard = OwnerComp.GetBlackboardComponent();
	m_pSurvivor = MyBTTUtils_RuniskovsDan::GetSurvivorPawn(OwnerComp);
	
	m_TargetPos = m_Blackboard->GetValueAsVector(m_TargetPoint.SelectedKeyName);
	
	m_pSteeringComponent = m_pSurvivor->GetComponentByClass<USteeringComponent_RuniskovsDan>();
	m_pSteeringComponent->SetBehavior<FSeek_RuniskovsDan>();
	
	m_pSteeringComponent->SetTarget(m_TargetPos);
	
	return EBTNodeResult::InProgress;
}

void UBTT_Seek_RuniskovsDan::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	if ((m_pSurvivor->GetActorLocation() - m_TargetPos).SquaredLength() <= KINDA_SMALL_NUMBER)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
