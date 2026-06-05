// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_EnterHouse_RuniskovsDan.generated.h"

class UHouseTrackerComponent_RuniskovsDan;
class AHouse;
class ASurvivorPawn;

UCLASS()
class RUNISKOVSDANZOMBIERUNTIME_API UBTT_EnterHouse_RuniskovsDan : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTT_EnterHouse_RuniskovsDan() { bNotifyTick = true; NodeName = "EnterHouse"; }
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	virtual uint16 GetInstanceMemorySize() const override;
	
private:
	UPROPERTY(EditAnywhere)
	float WaypointAcceptanceRadius{ 50.f };
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector ShouldLookAroundKey;
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector HouseKey;

	// --- Non Owning --- 
	AHouse* House{};
	ASurvivorPawn* Survivalist{};
	UHouseTrackerComponent_RuniskovsDan* HouseTracker{};

	void SaveHouseAsVisited() const noexcept;
};
