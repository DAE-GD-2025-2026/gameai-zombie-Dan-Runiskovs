// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_Blackboard.h"
#include "BTD_NeedsStamina_RuniskovsDan.generated.h"

UCLASS()
class RUNISKOVSDANZOMBIERUNTIME_API UBTD_NeedsStamina_RuniskovsDan : public UBTDecorator_Blackboard
{
	GENERATED_BODY()
public:
	UBTD_NeedsStamina_RuniskovsDan() { NodeName = "Needs Stamina"; }
	
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
