// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryManager_RuniskovsDan.h"
#include "Items/Weapon.h"

UInventoryManager_RuniskovsDan::UInventoryManager_RuniskovsDan()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UInventoryManager_RuniskovsDan::TryTake(ABaseItem& Item)
{
	// --- Full Inventory check ---
	if (IsFull()) return false;
	
	// --- Already has this item ---
	if (InventoryComponent->GetInventory().Contains(&Item)) return false;
	
	uint8_t SlotIdx{};
	while (!InventoryComponent->GrabItem(SlotIdx, &Item))
	{
		// --- Will pick up the item, trust me bro ---
		++SlotIdx;
	}

	++ItemCount;
	return true;
}

void UInventoryManager_RuniskovsDan::CleanUp() noexcept
{
	if (!InventoryComponent) return;

	const auto& Inventory{ InventoryComponent->GetInventory() };

	// --- Reverse order to avoid Index Shift
	for (int32 SlotIdx{ Inventory.Num() - 1 }; SlotIdx >= 0; --SlotIdx)
	{
		if (ABaseItem* Item{ Inventory[SlotIdx] };
			Item && Item->GetValue() == 0)
		{
			if (ItemCount > 0) --ItemCount;
		}
	}
}

void UInventoryManager_RuniskovsDan::BeginPlay()
{
	Super::BeginPlay();
	
	InventoryComponent = GetOwner()->FindComponentByClass<UInventoryComponent>();
	verify(InventoryComponent);
}

