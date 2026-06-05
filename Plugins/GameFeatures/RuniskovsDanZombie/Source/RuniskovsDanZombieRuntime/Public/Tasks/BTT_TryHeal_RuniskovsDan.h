// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_TryHeal_RuniskovsDan.generated.h"

UCLASS()
class RUNISKOVSDANZOMBIERUNTIME_API UBTT_TryHeal_RuniskovsDan : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTT_TryHeal_RuniskovsDan() { bNotifyTick = false; NodeName = "Try Heal"; };
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
