// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "Components/ActorComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "StudentPerceptor_RuniskovsDan.generated.h"

class ABaseItem;
class AActor;
class UHealthComponent;
class UHouseTrackerComponent_RuniskovsDan;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RUNISKOVSDANZOMBIERUNTIME_API UStudentPerceptor_RuniskovsDan : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStudentPerceptor_RuniskovsDan() { PrimaryComponentTick.bCanEverTick = true; }
	
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category = "Perception")
	FName HouseKeyName{ "House" };
	
	UPROPERTY(EditAnywhere, Category = "Perception")
	FName ItemKeyName{ "Item" };

	UPROPERTY(EditAnywhere, Category = "Perception")
	FBlackboardKeySelector ZombieKey;
	FName ZombieKeyName{ "Zombie" };

	UFUNCTION()
	virtual void OnPerceptionUpdated(AActor* Actor, const FAIStimulus Stimulus);
	
private:
	int OldHealth{};
	
	UPROPERTY()
	UBlackboardComponent* BlackboardComponent{};

	UHealthComponent* HealthComponent{};
	UHouseTrackerComponent_RuniskovsDan* HouseTrackerComponent{};

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;	
	
	// --- Helpers ---
	static uint8_t GetPriority(const ABaseItem& Item) noexcept;
	UHouseTrackerComponent_RuniskovsDan* GetHouseTracker() noexcept;
	bool ProcessHouseStimulus(AActor* PotentialHouse);
	bool ProcessItemStimulus(AActor* PotentialItem) const;
};
