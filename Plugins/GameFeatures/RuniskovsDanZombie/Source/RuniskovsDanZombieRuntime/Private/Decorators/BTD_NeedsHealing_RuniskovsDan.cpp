// Fill out your copyright notice in the Description page of Project Settings.


#include "Decorators/BTD_NeedsHealing_RuniskovsDan.h"

#include "Survivor/SurvivorPawn.h"
#include "Tasks/Utils_RuniskovsDan.h"

bool UBTD_NeedsHealing_RuniskovsDan::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const auto* Survivalist = MyBTTUtils_RuniskovsDan::GetSurvivorPawn(OwnerComp);
	verify(Survivalist);
	
	const auto* HealthComponent{ Survivalist->GetComponentByClass<UHealthComponent>() };
	verify(HealthComponent);
	
	const float HealthPercent =
		HealthComponent->GetHealth() /
		HealthComponent->GetMaxHealth();

	return HealthPercent < 0.5f;
}
