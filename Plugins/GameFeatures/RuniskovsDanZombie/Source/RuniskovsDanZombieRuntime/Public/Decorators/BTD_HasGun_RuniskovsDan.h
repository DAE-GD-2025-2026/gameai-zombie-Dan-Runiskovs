// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"

#include "BTD_HasGun_RuniskovsDan.generated.h"

/**
 * 
 */
UCLASS()
class RUNISKOVSDANZOMBIERUNTIME_API UBTD_HasGun_RuniskovsDan : public UBTDecorator
{
	GENERATED_BODY()
public:
	UBTD_HasGun_RuniskovsDan() { NodeName = "Has Gun"; }
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
