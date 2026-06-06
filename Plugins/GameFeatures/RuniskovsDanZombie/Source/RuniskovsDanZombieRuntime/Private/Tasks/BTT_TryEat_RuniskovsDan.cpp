// Fill out your copyright notice in the Description page of Project Settings.


#include "Tasks/BTT_TryEat_RuniskovsDan.h"
#include "Tasks/Utils_RuniskovsDan.h"
#include "Survivor/SurvivorPawn.h"
#include "Common/StaminaComponent.h"
#include "Components/InventoryManager_RuniskovsDan.h"
#include "Items/Food.h"

EBTNodeResult::Type UBTT_TryEat_RuniskovsDan::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto* SurvivorPawn{ MyBTTUtils_RuniskovsDan::GetSurvivorPawn(OwnerComp) };
	const auto* StaminaComponent{ SurvivorPawn->GetComponentByClass<UStaminaComponent>() };
	auto* InventoryManager{ SurvivorPawn->GetComponentByClass<UInventoryManager_RuniskovsDan>() };

	for (const auto FoodItems{ InventoryManager->TryGetItemsByType<AFood>() }; 
		auto* Item : FoodItems)
	{
		const auto FoodValue{ Item->GetValue() };
		if (const auto StaminaDiff{ StaminaComponent->GetMaxStamina() - StaminaComponent->GetCurrentStamina() };
			FoodValue <= StaminaDiff)
		{
			Item->UseItem(*SurvivorPawn);
			InventoryManager->CleanUp();
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
