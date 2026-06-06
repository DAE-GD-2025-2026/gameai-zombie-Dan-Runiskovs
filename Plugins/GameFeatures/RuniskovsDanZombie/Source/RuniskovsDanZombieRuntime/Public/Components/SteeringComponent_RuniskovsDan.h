// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <memory>
#include <typeindex>
#include <unordered_map>

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Steering/FSteeringBehaviorBase_RuniskovsDan.h"
#include "SteeringComponent_RuniskovsDan.generated.h"

class ASurvivorPawn;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RUNISKOVSDANZOMBIERUNTIME_API USteeringComponent_RuniskovsDan : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USteeringComponent_RuniskovsDan();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,FActorComponentTickFunction* ThisTickFunction) override;

	template<typename BehaviorType> requires std::derived_from<BehaviorType, FSteeringBehaviorBase_RuniskovsDan>
	void SetBehavior() noexcept { CurrentBehavior = BehaviorMap.at(typeid(BehaviorType)).get(); }
	
	void SetTarget(FVector const&) const;
	FVector GetOwnerLocation() const noexcept;
	void FaceTarget() const;
	
protected:
	virtual void BeginPlay() override;

private:
	std::unordered_map<std::type_index, std::unique_ptr<FSteeringBehaviorBase_RuniskovsDan>> BehaviorMap{};
	FSteeringBehaviorBase_RuniskovsDan* CurrentBehavior; // Non owning
	
	UPROPERTY(VisibleAnywhere)
	float AngularVelocity;
	
	ASurvivorPawn* Survivalist{}; // Not owning pointer
};
