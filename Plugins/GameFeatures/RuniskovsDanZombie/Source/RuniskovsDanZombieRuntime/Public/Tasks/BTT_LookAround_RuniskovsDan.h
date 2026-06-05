// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_LookAround_RuniskovsDan.generated.h"

class ASurvivorPawn;
class USteeringComponent_RuniskovsDan;

UCLASS()
class RUNISKOVSDANZOMBIERUNTIME_API UBTT_LookAround_RuniskovsDan : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTT_LookAround_RuniskovsDan() { bNotifyTick = true; NodeName = "LookAround"; }
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
private:
	UPROPERTY(EditAnywhere)
	float DegPerSec{ 160.f };
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector ShouldLookAroundKey;
    
	ASurvivorPawn* Survivalist{};
	USteeringComponent_RuniskovsDan* SteeringComponent{};
	//UHouseTrackerComponent_StukalovsAlex* HouseTrackerComponent{};
};
