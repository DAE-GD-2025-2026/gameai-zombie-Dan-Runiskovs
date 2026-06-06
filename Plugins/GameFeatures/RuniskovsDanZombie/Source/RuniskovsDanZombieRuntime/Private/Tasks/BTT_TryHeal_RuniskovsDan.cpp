// Fill out your copyright notice in the Description page of Project Settings.


#include "Tasks/BTT_TryHeal_RuniskovsDan.h"
#include "Tasks/Utils_RuniskovsDan.h"
#include "Survivor/SurvivorPawn.h"
#include "Common/HealthComponent.h"
#include "Components/InventoryManager_RuniskovsDan.h"
#include "Items/Medkit.h"

EBTNodeResult::Type UBTT_TryHeal_RuniskovsDan::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto* SurvivorPawn{ MyBTTUtils_RuniskovsDan::GetSurvivorPawn(OwnerComp) };
	const auto* HealthComponent{ SurvivorPawn->GetComponentByClass<UHealthComponent>() };
	auto* InventoryManager{ SurvivorPawn->GetComponentByClass<UInventoryManager_RuniskovsDan>() };

	for (auto MedKits{ InventoryManager->TryGetItemsByType<AMedkit>() }; 
		auto* MedKit : MedKits)
	{
		const auto HealthValue{ MedKit->GetValue() };
		if (const auto HealthDiff{  HealthComponent->GetMaxHealth() - HealthComponent->GetHealth() };
			HealthValue <= HealthDiff)
		{
			MedKit->UseItem(*SurvivorPawn);
			InventoryManager->CleanUp();
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
