// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_TryLoot_RuniskovsDan.generated.h"

class ASurvivorPawn;
class USteeringComponent_RuniskovsDan;
class ABaseItem;
class UInventoryManager_RuniskovsDan;

UCLASS()
class RUNISKOVSDANZOMBIERUNTIME_API UBTT_TryLoot_RuniskovsDan : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTT_TryLoot_RuniskovsDan() {bNotifyTick = true; NodeName = "Try Loot";}
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
private:
	ASurvivorPawn* Survivalist{};
	USteeringComponent_RuniskovsDan* Steering{};
	UInventoryManager_RuniskovsDan* Inventory{};
	
	TArray<ABaseItem*> ItemsToLoot;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector ShouldLookAroundKey;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector ItemKey;
	
	// --- Helpers ---
	bool TryUseItem(ABaseItem& Item, UBehaviorTreeComponent& OwnerComp) const;
	bool TryPushItem(ABaseItem* Item) noexcept;
	void SetItemAsTarget(const ABaseItem& Item) const noexcept;
	void UnsetBlackboardItem(UBehaviorTreeComponent& OwnerComp) const;
};
