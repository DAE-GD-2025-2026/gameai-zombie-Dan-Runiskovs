// Fill out your copyright notice in the Description page of Project Settings.


#include "Decorators/BTD_IsCloseEnough_RuniskovsDan.h"

#include "Survivor/SurvivorPawn.h"
#include "Tasks/Utils_RuniskovsDan.h"

bool UBTD_IsCloseEnough_RuniskovsDan::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const auto* Survivalist = MyBTTUtils_RuniskovsDan::GetSurvivorPawn(OwnerComp);
	if (!Survivalist) return false;

	const auto* BlackboardComponent{ OwnerComp.GetBlackboardComponent() };
	if (!BlackboardComponent) return false;

	UObject* TargetObject{ BlackboardComponent->GetValueAsObject(TargetKey.SelectedKeyName) };
	const AActor* TargetActor{ Cast<AActor>(TargetObject) };

	if (!TargetActor) return false;

	const double MaxDstSquared{ MaxDistance * MaxDistance };
	const double DstSquared{
		(TargetActor->GetActorLocation() - Survivalist->GetActorLocation()).SizeSquared()
	};

	return DstSquared < MaxDstSquared;
}
