// Fill out your copyright notice in the Description page of Project Settings.


#include "Decorators/BTD_IsCloseEnough_RuniskovsDan.h"

#include "Survivor/SurvivorPawn.h"
#include "Tasks/Utils_RuniskovsDan.h"

bool UBTD_IsCloseEnough_RuniskovsDan::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	// --- The regular shebang you already know ---
	const auto* Survivalist = MyBTTUtils_RuniskovsDan::GetSurvivorPawn(OwnerComp);
	verify(Survivalist);

	const auto* BlackboardComponent{ OwnerComp.GetBlackboardComponent() };
	verify(BlackboardComponent);

	// --- Is it something ? ---
	auto* TargetObject{ BlackboardComponent->GetValueAsObject(TargetKey.SelectedKeyName) };
	const auto* TargetActor{ Cast<AActor>(TargetObject) };
	if (!TargetActor) return false;

	// --- Distance check ---
	const double MaxDstSquared{ MaxDistance * MaxDistance };
	const double DstSquared{ (TargetActor->GetActorLocation() - Survivalist->GetActorLocation()).SizeSquared() };

	return DstSquared < MaxDstSquared;
}
