// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_Blackboard.h"
#include "BTD_IsCloseEnough_RuniskovsDan.generated.h"


UCLASS()
class RUNISKOVSDANZOMBIERUNTIME_API UBTD_IsCloseEnough_RuniskovsDan : public UBTDecorator_Blackboard
{
	GENERATED_BODY()
public:
	UBTD_IsCloseEnough_RuniskovsDan() { NodeName = "Is close enough"; }
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

private:
	UPROPERTY(EditAnywhere)
	double MaxDistance{ 69 };
	
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetKey;
};

