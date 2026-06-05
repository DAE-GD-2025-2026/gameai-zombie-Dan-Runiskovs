// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_TryEat_RuniskovsDan.generated.h"


UCLASS()
class RUNISKOVSDANZOMBIERUNTIME_API UBTT_TryEat_RuniskovsDan : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTT_TryEat_RuniskovsDan() { bNotifyTick = false; NodeName = "Try Eat";}
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
