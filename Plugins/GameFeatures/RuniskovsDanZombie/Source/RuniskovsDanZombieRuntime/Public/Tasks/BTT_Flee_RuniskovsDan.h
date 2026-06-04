// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_Flee_RuniskovsDan.generated.h"

class USteeringComponent_RuniskovsDan;
class ASurvivorPawn;

UCLASS()
class RUNISKOVSDANZOMBIERUNTIME_API UBTT_Flee_RuniskovsDan : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTT_Flee_RuniskovsDan() { bNotifyTick = true; NodeName = "Flee"; }
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
private:
	ASurvivorPawn* m_pSurvivor{};
	UBlackboardComponent* m_Blackboard{};
	
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector m_TargetPoint;

	FVector m_TargetPos;
	
	USteeringComponent_RuniskovsDan* m_pSteeringComponent{};
};
