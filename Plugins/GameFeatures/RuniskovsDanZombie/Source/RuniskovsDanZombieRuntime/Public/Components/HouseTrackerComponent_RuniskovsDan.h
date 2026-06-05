// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HouseTrackerComponent_RuniskovsDan.generated.h"

class AHouse;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RUNISKOVSDANZOMBIERUNTIME_API UHouseTrackerComponent_RuniskovsDan : public UActorComponent
{
	GENERATED_BODY()

public:
	UHouseTrackerComponent_RuniskovsDan();

	bool IsHouseVisited(const AHouse&) const noexcept;
	void SetHouseVisited(AHouse& House) noexcept;
	
protected:
	virtual void BeginPlay() override;

public:
	TArray<AHouse*> ArrHouses;	// All houses
	
	TArray<AHouse*> ArrVisitedHouses{};
	static constexpr uint8_t MaxVisited{6};
	uint8_t OldestHouseIdx{};

	void CollectHouses() noexcept;
	AHouse* GetCurrentHouse() noexcept;
	void MarkCurrentHouse() noexcept;
};
