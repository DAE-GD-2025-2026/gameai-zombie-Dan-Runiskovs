// Fill out your copyright notice in the Description page of Project Settings.


#include "Decorators/BTD_NeedsStamina_RuniskovsDan.h"

#include "Common/StaminaComponent.h"
#include "Survivor/SurvivorPawn.h"
#include "Tasks/Utils_RuniskovsDan.h"

bool UBTD_NeedsStamina_RuniskovsDan::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const auto* Survivalist = MyBTTUtils_RuniskovsDan::GetSurvivorPawn(OwnerComp);
	verify(Survivalist);
	
	const auto* StaminaComponent{ Survivalist->GetComponentByClass<UStaminaComponent>() };
	verify(StaminaComponent);
	
	const float StaminaPercent =
		StaminaComponent->GetCurrentStamina() /
		StaminaComponent->GetMaxStamina();

	return StaminaPercent <= 0.5f;
}
