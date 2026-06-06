// Fill out your copyright notice in the Description page of Project Settings.


#include "RuniskovsDanZombieRuntime/StudentPerceptor_RuniskovsDan.h"

#include "AIController.h"
#include "EngineUtils.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Common/HealthComponent.h"
#include "Components/HouseTrackerComponent_RuniskovsDan.h"
#include "Items/BaseItem.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Survivor/SurvivorPawn.h"
#include "Village/House/House.h"
#include "Zombies/BaseZombie.h"

void UStudentPerceptor_RuniskovsDan::BeginPlay()
{
	Super::BeginPlay();

	// --- Register Perception component ---
	if (const auto PerceptionComp = GetOwner()->GetComponentByClass<UAIPerceptionComponent>())
	{
		PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &UStudentPerceptor_RuniskovsDan::OnPerceptionUpdated);
	}
	
	// --- Register blackboard component ---
	if (auto* Controller{ Cast<AAIController>(GetOwner()->GetInstigatorController()) })
	{
		BlackboardComponent = Controller->GetBlackboardComponent();
		verify(BlackboardComponent);
	}

	// --- Loop over the houses, make them visible to the player ---
	for (const auto* House : TActorRange<AHouse>(GetWorld()))
	{
		if (auto* Source{ House->GetComponentByClass<UAIPerceptionStimuliSourceComponent>() })
		{
			Source->RegisterForSense(UAISense_Sight::StaticClass());
			Source->RegisterWithPerceptionSystem();
		}
	}

	// --- Save Initial health ---
	HealthComponent = GetOwner()->FindComponentByClass<UHealthComponent>();
	verify(HealthComponent);
	
	OldHealth = HealthComponent->GetHealth();
}

void UStudentPerceptor_RuniskovsDan::OnPerceptionUpdated(AActor* Actor, const FAIStimulus Stimulus)
{
	// --- Safety check ---
	if (!Stimulus.WasSuccessfullySensed() || Stimulus.IsExpired()) return;

	// --- Process visual stimulus ---
	if (Stimulus.Type == UAISense_Sight::GetSenseID<UAISense_Sight>())
	{
		// --- This one house ? -> return ---
		if (ProcessHouseStimulus(Actor)) return;

		// --- This one Item ? -> return ---
		if (ProcessItemStimulus(Actor)) return;

		// --- Not doing extra for the zombie, coz getting rid of it in any case ---
		if (auto* Zombie{ Cast<ABaseZombie>(Actor) }; Zombie)
		{
			BlackboardComponent->SetValueAsObject(ZombieKeyName, Zombie);
		}
	}
}

void UStudentPerceptor_RuniskovsDan::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// --- Could use damage stimuli here, but peers said it didn't work ---
	if (HealthComponent->GetHealth() == OldHealth) return;
	
	// --- Save new health ---
	OldHealth = HealthComponent->GetHealth();
	
	// --- Get all zombies ---
	TArray<AActor*> Zombies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseZombie::StaticClass(), Zombies);
	
	// --- Get the survivalist ---
	const auto* Survivalist{ Cast<ASurvivorPawn>(GetOwner())};
	
	// --- Find the closest one ---
	if(!Zombies.IsEmpty())
	{
		ABaseZombie* ClosestZombie{nullptr};
		
		for (auto* Zombie : Zombies)
		{
			if (!ClosestZombie)
			{
				ClosestZombie = Cast<ABaseZombie>(Zombie);
				continue;
			}
			
			if (Zombie->GetDistanceTo(Survivalist) < ClosestZombie->GetDistanceTo(Survivalist))
			{
				ClosestZombie = Cast<ABaseZombie>(Zombie);
			}
		}
		// --- Save it to the blackboard ---
		BlackboardComponent->SetValueAsObject(ZombieKey.SelectedKeyName, ClosestZombie);
	}
}

uint8_t UStudentPerceptor_RuniskovsDan::GetPriority(const ABaseItem& Item) noexcept
{
	switch (Item.GetItemType())
	{
	case EItemType::Shotgun:
		return 4u;
	case EItemType::Pistol:
		return 3u;
	case EItemType::Medkit:
		return 2u;
	case EItemType::Food:
		return 2u;		
	default: 
		return 0u;
	}
}

UHouseTrackerComponent_RuniskovsDan* UStudentPerceptor_RuniskovsDan::GetHouseTracker() noexcept
{
	if (!HouseTrackerComponent)
	{
		HouseTrackerComponent = GetOwner()->FindComponentByClass<UHouseTrackerComponent_RuniskovsDan>();
	}

	return HouseTrackerComponent;
}

bool UStudentPerceptor_RuniskovsDan::ProcessHouseStimulus(AActor* PotentialHouse)
{
	auto* House{ Cast<AHouse>(PotentialHouse) };
	if (!House) return false;

	const auto* Tracker{ GetHouseTracker() };
	if (!Tracker) return false;

	if (!BlackboardComponent) return false;

	// --- Skip if this one has been visited ---
	if (Tracker->IsHouseVisited(*House)) return true;
	
	if (auto* CurrentHouse{ Cast<AHouse>(BlackboardComponent->GetValueAsObject(HouseKeyName)) })
	{
		// --- Get both distances ---
		const auto NewHouseDistance{ GetOwner()->GetDistanceTo(House) };
		const auto CurrentHouseDistance{ GetOwner()->GetDistanceTo(CurrentHouse) };

		// --- Is this one closer ? ---
		House =
			NewHouseDistance < CurrentHouseDistance
			? House
			: CurrentHouse;
	}

	BlackboardComponent->SetValueAsObject(HouseKeyName, House);
	return true;
}

bool UStudentPerceptor_RuniskovsDan::ProcessItemStimulus(AActor* PotentialItem) const
{
	if (auto* Item{ Cast<ABaseItem>(PotentialItem) }; Item)
	{
		// --- Check the priority of queued item---
		if (auto* CurrentItem{ (Cast<ABaseItem>(BlackboardComponent->GetValueAsObject(ItemKeyName))) })
		{
			Item = 
				(GetPriority(*CurrentItem) < GetPriority(*Item)) 
				? CurrentItem 
				: Item;
		}

		// --- Save it ---
		BlackboardComponent->SetValueAsObject(ItemKeyName, Item);
		return true;
	}
	return false;
}
