// Fill out your copyright notice in the Description page of Project Settings.


#include "Decorators/BTD_HasGun_RuniskovsDan.h"

#include "Components/InventoryManager_RuniskovsDan.h"
#include "Items/Pistol.h"
#include "Items/Shotgun.h"
#include "Tasks/Utils_RuniskovsDan.h"

bool UBTD_HasGun_RuniskovsDan::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const auto* Survivalist = MyBTTUtils_RuniskovsDan::GetSurvivorPawn(OwnerComp);

	if (!Survivalist) return false;

	const auto* Inventory{ Survivalist->GetComponentByClass<UInventoryManager_RuniskovsDan>() };

	if (!Inventory) return false;
	
	const bool HasPistol{ Inventory->GetItemsByType<APistol>().Num() > 0 };

	const bool HasShotgun{ Inventory->GetItemsByType<AShotgun>().Num() > 0 };

	return HasPistol || HasShotgun;
}
