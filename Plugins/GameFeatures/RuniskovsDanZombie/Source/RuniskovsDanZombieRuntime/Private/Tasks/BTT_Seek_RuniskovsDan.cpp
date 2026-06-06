// Fill out your copyright notice in the Description page of Project Settings.


#include "Tasks/BTT_Seek_RuniskovsDan.h"

#include "Tasks/Utils_RuniskovsDan.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Survivor/SurvivorPawn.h"
#include "Components/SteeringComponent_RuniskovsDan.h"


EBTNodeResult::Type UBTT_Seek_RuniskovsDan::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
 	Blackboard = OwnerComp.GetBlackboardComponent();
	Survivalist = MyBTTUtils_RuniskovsDan::GetSurvivorPawn(OwnerComp);
	
	TargetPosition = Blackboard->GetValueAsVector(TargetPoint.SelectedKeyName);
	
	SteeringComponent = Survivalist->GetComponentByClass<USteeringComponent_RuniskovsDan>();
	SteeringComponent->SetBehavior<FSeek_RuniskovsDan>();
	
	SteeringComponent->SetTarget(TargetPosition);
	
	return EBTNodeResult::InProgress;
}

void UBTT_Seek_RuniskovsDan::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	if ((Survivalist->GetActorLocation() - TargetPosition).SquaredLength() <= KINDA_SMALL_NUMBER)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
