// Fill out your copyright notice in the Description page of Project Settings.


#include "Tasks/BTT_Wander_RuniskovsDan.h"

#include "Utils_RuniskovsDan.h"
#include "Components/SteeringComponent_RuniskovsDan.h"
#include "Survivor/SurvivorPawn.h"

EBTNodeResult::Type UBTT_Wander_RuniskovsDan::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ASurvivorPawn* SurvivorPawn = MyBTTUtils_RuniskovsDan::GetSurvivorPawn(OwnerComp);
	verify(SurvivorPawn);
	
	USteeringComponent_RuniskovsDan* SteeringComponent{ SurvivorPawn->GetComponentByClass<USteeringComponent_RuniskovsDan>() };
	verify(SteeringComponent);
	SteeringComponent->SetBehavior<FWander_RuniskovsDan>();
	
	return EBTNodeResult::Succeeded;
}
