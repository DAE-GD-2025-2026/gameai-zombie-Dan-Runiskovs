// Fill out your copyright notice in the Description page of Project Settings.


#include "Tasks/BTT_TryLoot_RuniskovsDan.h"

#include "Utils_RuniskovsDan.h"
#include "Components/InventoryManager_RuniskovsDan.h"
#include "Components/SteeringComponent_RuniskovsDan.h"
#include "Survivor/SurvivorPawn.h"

EBTNodeResult::Type UBTT_TryLoot_RuniskovsDan::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// --- Pickup cached item ---
	auto* Item{ MyBTTUtils_RuniskovsDan::GetFromBlackboard<ABaseItem>(OwnerComp, TEXT("Item")) };
	TryPushItem(Item);
	if (ItemsToLoot.IsEmpty())
	{
		auto& Blackboard{ MyBTTUtils_RuniskovsDan::GetBlackboard(OwnerComp) };
		Blackboard.SetValueAsBool(ShouldLookAroundKey.SelectedKeyName, true);
		return EBTNodeResult::Failed;
	}
	
	// --- Make Survivalist Seek ---
	Survivalist = MyBTTUtils_RuniskovsDan::GetSurvivorPawn(OwnerComp);
	verify(Survivalist);
	
	// --- Get Inventory ---
	Inventory = Survivalist->FindComponentByClass<UInventoryManager_RuniskovsDan>();
	verify(Inventory);

	// --- Get Steering ---
	Steering = Survivalist->GetComponentByClass<USteeringComponent_RuniskovsDan>();
	verify(Steering);
	
	Steering->SetBehavior<FSeek_RuniskovsDan>();
	
	return EBTNodeResult::InProgress;
}

void UBTT_TryLoot_RuniskovsDan::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	// --- Inventory Full check ---
	if (Inventory->IsFull()) { FinishLatentTask(OwnerComp, EBTNodeResult::Failed); return; }

	// --- There are no queued items to pick up ---
	if (ItemsToLoot.IsEmpty()) { FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded); return; }

	// --- Queue next item to loot ---
	auto* Item{ ItemsToLoot.Last() };
	
	// --- Use or loot ---
	const auto DirToItem{ Item->GetActorLocation() - Survivalist->GetActorLocation() };
	if (const auto Range{ Inventory->GetPickupRange() }; DirToItem.SquaredLength() < Range * Range)
	{
		// --- Can't use the item? ---
		if (!TryUseItem(*Item, OwnerComp))
		{
			// --- Try take it ---
			if (!Inventory->TryTake(*Item))
			{
				ItemsToLoot.Empty();
				FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
				return;
			}
			UnsetBlackboardItem(OwnerComp);
		}
		
		ItemsToLoot.Pop();
		
		auto& Blackboard = MyBTTUtils_RuniskovsDan::GetBlackboard(OwnerComp);

		Blackboard.SetValueAsBool(ShouldLookAroundKey.SelectedKeyName,true);
		
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else
	{
		// --- Too far? Move! ---
		SetItemAsTarget(*Item);		
	}
}

bool UBTT_TryLoot_RuniskovsDan::TryUseItem(ABaseItem& Item, UBehaviorTreeComponent& OwnerComp) const
{
	switch (Item.GetItemType())
	{
	case EItemType::Food:
		{
			// --- Eat the thing --- 
			const auto* StaminaComponent{ Survivalist->GetComponentByClass<UStaminaComponent>() };
			verify(StaminaComponent);
		
			const auto FoodValue{ Item.GetValue() };
			const auto StaminaDiff{ StaminaComponent->GetMaxStamina() - StaminaComponent->GetCurrentStamina() };
			if (FoodValue <= StaminaDiff)
			{
				Item.UseItem(*Survivalist);
				UnsetBlackboardItem(OwnerComp);
				return true;
			}
			return false;
		}
		
	case EItemType::Medkit:
		{
			// --- Try Heal Up ---
			const auto* HealthComponent{ Survivalist->GetComponentByClass<UHealthComponent>() };
			verify(HealthComponent);
		
			const auto HealthValue{ Item.GetValue() };
			const auto HealthDiff{  HealthComponent->GetMaxHealth() - HealthComponent->GetHealth() };
			if (HealthValue <= HealthDiff)
			{
				Item.UseItem(*Survivalist);
				UnsetBlackboardItem(OwnerComp);
				return true;
			}
			return false;
		}
		
	case EItemType::Garbage:
		Item.Destroy();
		UnsetBlackboardItem(OwnerComp);
		return true;
	default: 
		// --- Guns? No! ---
		return false;
	}
}

bool UBTT_TryLoot_RuniskovsDan::TryPushItem(ABaseItem* Item) noexcept
{
	// --- Safety check ---
	if (!Item) return false;
	
	// --- Check if said item is already in the list ---
	if (ItemsToLoot.Contains(Item)) return false;
	
	ItemsToLoot.Push(Item);
	return true;
}

void UBTT_TryLoot_RuniskovsDan::SetItemAsTarget(const ABaseItem& Item) const noexcept
{
	const auto ItemLocation{ Item.GetActorLocation() };
	Steering->SetTarget(ItemLocation);
}

void UBTT_TryLoot_RuniskovsDan::UnsetBlackboardItem(UBehaviorTreeComponent& OwnerComp) const
{
	auto* BlackboardComponent{ OwnerComp.GetBlackboardComponent() };
	verify(BlackboardComponent);
	
	return BlackboardComponent->SetValueAsObject(ItemKey.SelectedKeyName, nullptr);
}
