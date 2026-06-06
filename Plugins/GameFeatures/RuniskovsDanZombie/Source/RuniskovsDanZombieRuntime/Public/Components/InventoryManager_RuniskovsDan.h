// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Common/InventoryComponent.h"
#include "InventoryManager_RuniskovsDan.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RUNISKOVSDANZOMBIERUNTIME_API UInventoryManager_RuniskovsDan : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryManager_RuniskovsDan() { PrimaryComponentTick.bCanEverTick = false; }
	
	bool TryTake(ABaseItem& Item);
	void CleanUp() noexcept;

	template <typename T> requires std::derived_from<T, ABaseItem>
	TArray<T*> TryGetItemsByType() const noexcept
	{
		TArray Inventory{ InventoryComponent->GetInventory() };
		
		TArray<T*> Result;
		for (auto* Item : Inventory)
		{
			if (T* CastedItem = Cast<T>(Item)) Result.Add(CastedItem);
		}
    
		return Result;
	}

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UInventoryComponent* InventoryComponent{};
	uint8_t ItemCount{};
	
	float GetPickupRange() const noexcept { return InventoryComponent->GetPickupRange(); }
	bool IsFull() const noexcept { return ItemCount == InventoryComponent->GetInventoryCapacity(); }
};
