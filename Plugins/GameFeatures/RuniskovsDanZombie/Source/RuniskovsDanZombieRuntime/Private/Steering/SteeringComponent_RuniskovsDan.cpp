// Fill out your copyright notice in the Description page of Project Settings.


#include "Steering/SteeringComponent_RuniskovsDan.h"
#include "Survivor/SurvivorPawn.h"
#include "Steering/FSteeringBehaviorBase_RuniskovsDan.h"


// Sets default values for this component's properties
USteeringComponent_RuniskovsDan::USteeringComponent_RuniskovsDan()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	m_behaviorMap.emplace(typeid(FSeek_RuniskovsDan), std::make_unique<FSeek_RuniskovsDan>());
	m_pCurrentBehavior = m_behaviorMap.at(typeid(FSeek_RuniskovsDan)).get();
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
	
	m_pSurvivorPawn->AddMovementInput(linearVelocity.LinearVelocity);
	
	const FVector moveDir{ linearVelocity.LinearVelocity };
	m_pSurvivorPawn->SetActorRotation(moveDir.Rotation());
}

void USteeringComponent_RuniskovsDan::SetTarget(FVector const& target) const
{
	m_pCurrentBehavior->SetTarget(target);
}

FVector USteeringComponent_RuniskovsDan::GetOwnerLocation() const noexcept
{
	return m_pSurvivorPawn->GetActorLocation();
}

