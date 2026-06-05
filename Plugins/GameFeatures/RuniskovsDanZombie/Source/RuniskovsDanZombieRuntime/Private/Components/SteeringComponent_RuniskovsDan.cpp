// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SteeringComponent_RuniskovsDan.h"
#include "Survivor/SurvivorPawn.h"
#include "Steering/FSteeringBehaviorBase_RuniskovsDan.h"


// Sets default values for this component's properties
USteeringComponent_RuniskovsDan::USteeringComponent_RuniskovsDan()
{
	PrimaryComponentTick.bCanEverTick = true;

	m_behaviorMap.emplace(typeid(FSeek_RuniskovsDan), std::make_unique<FSeek_RuniskovsDan>());
	m_behaviorMap.emplace(typeid(FFlee_RuniskovsDan), std::make_unique<FFlee_RuniskovsDan>());
	m_behaviorMap.emplace(typeid(FWander_RuniskovsDan), std::make_unique<FWander_RuniskovsDan>());

	m_pCurrentBehavior = m_behaviorMap.at(typeid(FWander_RuniskovsDan)).get();
}


// Called when the game starts
void USteeringComponent_RuniskovsDan::BeginPlay()
{
	Super::BeginPlay();

	m_pSurvivorPawn = CastChecked<ASurvivorPawn>(GetOwner());
}


// Called every frame
void USteeringComponent_RuniskovsDan::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const auto linearVelocity = m_pCurrentBehavior->CalculateSteering(DeltaTime, *this);
	FVector velocity{ linearVelocity.LinearVelocity.X, linearVelocity.LinearVelocity.Y, 0};
	
	m_pSurvivorPawn->AddMovementInput(velocity);
	
	const FVector moveDir{ velocity };
	m_pSurvivorPawn->SetActorRotation(moveDir.Rotation());
}

void USteeringComponent_RuniskovsDan::SetTarget(FVector const& target) const
{
	FVector2D Target2D{ target.X, target.Y };
	m_pCurrentBehavior->SetTarget(Target2D);
}

FVector USteeringComponent_RuniskovsDan::GetOwnerLocation() const noexcept
{
	return m_pSurvivorPawn->GetActorLocation();
}

