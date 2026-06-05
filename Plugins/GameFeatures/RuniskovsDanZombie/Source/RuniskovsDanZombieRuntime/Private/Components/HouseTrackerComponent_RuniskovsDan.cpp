// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HouseTrackerComponent_RuniskovsDan.h"

#include "Kismet/GameplayStatics.h"
#include "Tasks/Utils_RuniskovsDan.h"
#include "Village/House/House.h"


// Sets default values for this component's properties
UHouseTrackerComponent_RuniskovsDan::UHouseTrackerComponent_RuniskovsDan()
{
	PrimaryComponentTick.bCanEverTick = true;

	// No houses visited -> nullptr
	ArrVisitedHouses.Reserve(MaxVisited);
	std::fill(ArrVisitedHouses.begin(), ArrVisitedHouses.end(), nullptr);
}


// Called when the game starts
void UHouseTrackerComponent_RuniskovsDan::BeginPlay()
{
	Super::BeginPlay();
	CollectHouses();
}

void UHouseTrackerComponent_RuniskovsDan::CollectHouses() noexcept
{
	TArray<AActor*> PotentialHouses;
	UGameplayStatics::GetAllActorsOfClass(GetOwner()->GetWorld(), AHouse::StaticClass(), PotentialHouses);
	
	// --- Safety check ---
	for (AActor* Actor : PotentialHouses)
	{
		if (AHouse* House = Cast<AHouse>(Actor)) ArrHouses.Add(House);
	}
}

bool UHouseTrackerComponent_RuniskovsDan::IsHouseVisited(const AHouse& House) const noexcept
{
	return ArrVisitedHouses.Contains(&House);
}

void UHouseTrackerComponent_RuniskovsDan::SetHouseVisited(AHouse& House) noexcept
{
	// --- Skip if saved ---
	if (IsHouseVisited(House)) return;
	
	// --- Save --- 
	ArrVisitedHouses[OldestHouseIdx] = &House;
	
	// --- Inc and round oldest house idx ---
	++OldestHouseIdx;
	OldestHouseIdx %= MaxVisited;
}

AHouse* UHouseTrackerComponent_RuniskovsDan::GetCurrentHouse() noexcept
{
	// --- Get Survivalist position ---
	if (!GetOwner()) return nullptr;
	const FVector OwnerLocation = GetOwner()->GetActorLocation();
    
	// --- Check in bounds of each house ---
	for (AHouse* House : ArrHouses)
	{
		if (!IsValid(House)) continue;

		if (const auto HouseBounds{MyBTTUtils_RuniskovsDan::GetHouseBounds(House)};
			HouseBounds.IsInsideXY(OwnerLocation)) return House;
	}
    
	// --- Not inside a house? ---
	return nullptr;
}

void UHouseTrackerComponent_RuniskovsDan::MarkCurrentHouse() noexcept
{
	// --- Try get current house ---
	AHouse* const House{ GetCurrentHouse() };
	if (!House) return;
	
	SetHouseVisited(*House);
}
