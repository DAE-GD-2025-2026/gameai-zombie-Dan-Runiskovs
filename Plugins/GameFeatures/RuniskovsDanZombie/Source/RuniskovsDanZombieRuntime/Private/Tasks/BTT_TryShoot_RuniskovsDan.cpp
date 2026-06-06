// Fill out your copyright notice in the Description page of Project Settings.


#include "Tasks/BTT_TryShoot_RuniskovsDan.h"

#include "Tasks/Utils_RuniskovsDan.h"
#include "Items/Pistol.h"
#include "Items/Shotgun.h"
#include "Components/SteeringComponent_RuniskovsDan.h"
#include "Components/InventoryManager_RuniskovsDan.h"
#include "Steering/FSteeringBehaviorBase_RuniskovsDan.h"
#include "Zombies/BaseZombie.h"
#include "Survivor/SurvivorPawn.h"

EBTNodeResult::Type UBTT_TryShoot_RuniskovsDan::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{	
	// --- Get current zombie target ---
	Zombie = MyBTTUtils_RuniskovsDan::GetFromBlackboard<ABaseZombie>(OwnerComp, TEXT("Zombie"));
	verify(Zombie)
	ZombieHealthComponent = Zombie->GetComponentByClass<UHealthComponent>();
	verify(ZombieHealthComponent);
	
	// --- Get the survivalist ---
	Survivalist = MyBTTUtils_RuniskovsDan::GetSurvivorPawn(OwnerComp);
	
	SteeringComponent = Survivalist->GetComponentByClass<USteeringComponent_RuniskovsDan>();
	verify(SteeringComponent);
	SteeringComponent->SetBehavior<FLookAt_RuniskovsDan>();

	// --- Get Inventory ---
	Inventory = Survivalist->GetComponentByClass<UInventoryManager_RuniskovsDan>();
	verify(Inventory);
	// --- Try get any gun ---
	if (!TryGetAnyGun()) return EBTNodeResult::Failed;

	// --- Set Zombie as Target ---
	const auto ZombieLocation{ Zombie->GetActorLocation() };
	SteeringComponent->SetTarget(ZombieLocation);
	SteeringComponent->FaceTarget();
	
	// --- Here We have guns ---
	GetWeapon(OwnerComp);
	
	Shoot();
	
	UBlackboardComponent& BlackboardComponent{ MyBTTUtils_RuniskovsDan::GetBlackboard(OwnerComp) };
	BlackboardComponent.SetValueAsObject(ZombieKey.SelectedKeyName, nullptr);
	
	Inventory->CleanUp();

	return EBTNodeResult::Succeeded;
}

bool UBTT_TryShoot_RuniskovsDan::TryGetAnyGun()
{
	Shotgun = nullptr;
	Pistol = nullptr;
	for (const auto Items{ MyBTTUtils_RuniskovsDan::GetInventory(*Survivalist) }; auto* Item : Items)
	{
		if (Shotgun == nullptr)
		{
			Shotgun = Cast<AShotgun>(Item);
			if (Shotgun) continue;
		}
		
		if (Pistol == nullptr) Pistol = Cast<APistol>(Item);

		// --- Got both guns ---
		if (Shotgun && Pistol) break;
	}
	
	return Pistol or Shotgun;
}

void UBTT_TryShoot_RuniskovsDan::GetWeapon(UBehaviorTreeComponent& OwnerComp) noexcept
{
	// --- Face The zombie ---
	auto DirToZombie{ Zombie->GetActorLocation() - Survivalist->GetActorLocation() };
	DirToZombie.Normalize();

	constexpr auto MaxShotgunDistance{ 100.f }; // Up close and personal
	constexpr auto ShotgunDstSq{MaxShotgunDistance * MaxShotgunDistance};
	const auto DstToZombieSq{DirToZombie.SquaredLength()};
	
	if (Shotgun and (DstToZombieSq <= ShotgunDstSq))
	{
		CurrentGun = Shotgun;
	}
	else
	{
		// --- Too far for a shotgun ---
		CurrentGun = Pistol;
	}

	// --- Too far and no pistol ---
	if (CurrentGun == nullptr) FinishLatentTask(OwnerComp,EBTNodeResult::Failed);

	// --- No Guns ---
	if (!(Pistol || Shotgun)) FinishLatentTask(OwnerComp,EBTNodeResult::Failed);
	
}

void UBTT_TryShoot_RuniskovsDan::Shoot() const noexcept
{
	CurrentGun->UseItem(*Survivalist);
}