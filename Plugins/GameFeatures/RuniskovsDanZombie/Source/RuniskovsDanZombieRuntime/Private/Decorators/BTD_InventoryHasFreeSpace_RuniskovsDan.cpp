// Fill out your copyright notice in the Description page of Project Settings.


#include "Decorators/BTD_InventoryHasFreeSpace_RuniskovsDan.h"

#include "Components/InventoryManager_RuniskovsDan.h"
#include "Survivor/SurvivorPawn.h"
#include "Tasks/Utils_RuniskovsDan.h"

bool UBTD_InventoryHasFreeSpace_RuniskovsDan::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const auto* Survivalist = MyBTTUtils_RuniskovsDan::GetSurvivorPawn(OwnerComp);
	verify(Survivalist);

	const auto* BlackboardComponent{ OwnerComp.GetBlackboardComponent() };
	verify(BlackboardComponent);

	const auto* InventoryManager{ Survivalist->GetComponentByClass<UInventoryManager_RuniskovsDan>() };
	verify(InventoryManager);
	
	return !(InventoryManager->IsFull());
}
