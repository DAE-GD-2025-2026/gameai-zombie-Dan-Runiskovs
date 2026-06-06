// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_TryShoot_RuniskovsDan.generated.h"

class ASurvivorPawn;
class ABaseZombie;
class UHealthComponent;
class USteeringComponent_RuniskovsDan;
class UInventoryManager_RuniskovsDan;
class ABaseItem;
class AShotgun;
class APistol;

UCLASS()
class RUNISKOVSDANZOMBIERUNTIME_API UBTT_TryShoot_RuniskovsDan : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTT_TryShoot_RuniskovsDan() {bNotifyTick = false; NodeName = "Try Shoot";}
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	// --- Shotgun damage: 2 per pallet ---
	// --- Pistol damage: 5 ---

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector ZombieKey;

	ASurvivorPawn* Survivalist{};
	UInventoryManager_RuniskovsDan* Inventory{};
	USteeringComponent_RuniskovsDan* SteeringComponent{};
	
	ABaseZombie* Zombie{};
	UHealthComponent* ZombieHealthComponent{};
	
	ABaseItem* CurrentGun{};
	AShotgun* Shotgun{};
	APistol* Pistol{};
	
	bool TryGetAnyGun();

	void GetWeapon(UBehaviorTreeComponent& OwnerComp) noexcept;
	void Shoot() const noexcept;
};
