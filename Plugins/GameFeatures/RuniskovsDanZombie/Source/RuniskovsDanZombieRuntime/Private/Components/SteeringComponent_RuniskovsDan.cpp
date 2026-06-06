// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SteeringComponent_RuniskovsDan.h"
#include "Survivor/SurvivorPawn.h"
#include "Steering/FSteeringBehaviorBase_RuniskovsDan.h"


// Sets default values for this component's properties
USteeringComponent_RuniskovsDan::USteeringComponent_RuniskovsDan()
{
	PrimaryComponentTick.bCanEverTick = true;

	BehaviorMap.emplace(typeid(FSeek_RuniskovsDan), std::make_unique<FSeek_RuniskovsDan>());
	BehaviorMap.emplace(typeid(FFlee_RuniskovsDan), std::make_unique<FFlee_RuniskovsDan>());
	BehaviorMap.emplace(typeid(FLookAt_RuniskovsDan), std::make_unique<FLookAt_RuniskovsDan>());
	BehaviorMap.emplace(typeid(FWander_RuniskovsDan), std::make_unique<FWander_RuniskovsDan>());

	CurrentBehavior = BehaviorMap.at(typeid(FWander_RuniskovsDan)).get();
}


// Called when the game starts
void USteeringComponent_RuniskovsDan::BeginPlay()
{
	Super::BeginPlay();

	// --- No utils here, coz no OwnerComp yet, gotta put in some elbow grease ---
	Survivalist = CastChecked<ASurvivorPawn>(GetOwner());
}


// Called every frame
void USteeringComponent_RuniskovsDan::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// --- Calculate steering ---
	const auto Steering{ CurrentBehavior->CalculateSteering(DeltaTime, *this) };
	const FVector Velocity{ Steering.LinearVelocity.X, Steering.LinearVelocity.Y, 0};
	
	Survivalist->AddMovementInput(Velocity);
	
	// --- Rotate him as well ---
	const FVector MoveDir{ Velocity };
	Survivalist->SetActorRotation(MoveDir.Rotation());
}

void USteeringComponent_RuniskovsDan::SetTarget(const FVector& Target) const
{
	const FVector2D Target2D{ Target.X, Target.Y };
	CurrentBehavior->SetTarget(Target2D);
}

FVector USteeringComponent_RuniskovsDan::GetOwnerLocation() const noexcept
{
	return Survivalist->GetActorLocation();
}

void USteeringComponent_RuniskovsDan::FaceTarget() const
{
	const auto Target{ CurrentBehavior->GetTarget() };
    
	// --- Calculate From and To ---
	const auto CurrentLocation{ Survivalist->GetActorLocation()};
	const FVector TargetLocation{ Target.X, Target.Y, CurrentLocation.Z };
    
	// --- Calculate Rotation ---
	const FRotator NewRotation{ (TargetLocation - CurrentLocation).Rotation() };
    
	Survivalist->SetActorRotation(NewRotation);
}

