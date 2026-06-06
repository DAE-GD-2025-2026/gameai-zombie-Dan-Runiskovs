// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_Blackboard.h"
#include "BTD_InventoryHasFreeSpace_RuniskovsDan.generated.h"

UCLASS()
class RUNISKOVSDANZOMBIERUNTIME_API UBTD_InventoryHasFreeSpace_RuniskovsDan : public UBTDecorator_Blackboard
{
	GENERATED_BODY()
public:
	UBTD_InventoryHasFreeSpace_RuniskovsDan() { NodeName = "Inv. has free space"; }
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
