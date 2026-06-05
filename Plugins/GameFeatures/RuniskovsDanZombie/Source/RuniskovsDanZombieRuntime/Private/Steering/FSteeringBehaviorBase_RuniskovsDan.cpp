#include "Steering/FSteeringBehaviorBase_RuniskovsDan.h"

#include "Components/SteeringComponent_RuniskovsDan.h"
#include "Tasks/Utils_RuniskovsDan.h"

FSteeringOutput FSeek_RuniskovsDan::CalculateSteering(float DeltaT, USteeringComponent_RuniskovsDan& Component)
{
	FSteeringOutput Steering{};
	constexpr float CloseDstSquared{25.f};
	
	// --- Get Actor Position ---
	const FVector2D Postion{ MyBTTUtils_RuniskovsDan::GetSurvivalistLocation(Component) };
	
	// --- Calculate Linear Velocity ---
	Steering.LinearVelocity = Target - Postion;
	
	// --- Too close? ---
	if (Steering.LinearVelocity.SquaredLength() <= CloseDstSquared)
		Steering.LinearVelocity = FVector2D::ZeroVector;	
	
	// --- Weird case protection ---
	if (!Steering.LinearVelocity.IsNearlyZero())
		Steering.LinearVelocity.Normalize();
	
	return Steering;
}

FSteeringOutput FFlee_RuniskovsDan::CalculateSteering(float DeltaT, USteeringComponent_RuniskovsDan& Component)
{
	FSteeringOutput Steering{};
	
	// --- Get Actor Position ---
	const FVector2D Postion{ MyBTTUtils_RuniskovsDan::GetSurvivalistLocation(Component) };
	
	// --- Calculate Linear Velocity ---
	Steering.LinearVelocity = -(Target - Postion);
	
	// --- Weird case protection ---
	if (!Steering.LinearVelocity.IsNearlyZero())
		Steering.LinearVelocity.Normalize();
	
	return Steering; 
}

FSteeringOutput FWander_RuniskovsDan::CalculateSteering(float DeltaT, USteeringComponent_RuniskovsDan& Component)
{
	FSteeringOutput Steering{};
	
	// --- Calculate and round new angle ---
	float NewAngleDeg{ LastAngleDeg + FMath::FRandRange(-MaxOffset, MaxOffset)};
	NewAngleDeg = FMath::Fmod(FMath::Fmod(NewAngleDeg, 360.0f) + 360.0f, 360.0f);
	
	// --- Get Forward ---
	AActor* Survivalist{ Component.GetOwner() };
	verify(Survivalist);
	FVector2D const ForwardVector{ Survivalist->GetActorForwardVector().X , Survivalist->GetActorForwardVector().Y };

	// --- Save the angle --- 
	LastAngleDeg = NewAngleDeg;
	
	// --- Set new target ---
	FVector const AgentLocation{ Survivalist->GetActorLocation() };
	FVector2D const NewTargetLocation{ 
		FVector2D{AgentLocation.X, AgentLocation.Y} + TargetCircleOffset  * ForwardVector 
		+ TargetCircleRadius * FVector2D(
			FMath::Cos(FMath::DegreesToRadians(NewAngleDeg)), 
			FMath::Sin(FMath::DegreesToRadians(NewAngleDeg))
			) 
	};
	
	// --- Set New Target ---
	Target = NewTargetLocation;
	
	// --- Calculate Steering ---
	return FSeek_RuniskovsDan::CalculateSteering(DeltaT, Component);
}

FSteeringOutput FLookAt_RuniskovsDan::CalculateSteering(float DeltaT, USteeringComponent_RuniskovsDan& Component)
{
	FSteeringOutput Steering{};
	
	// --- Get Actor Position ---
	const FVector2D Postion{ MyBTTUtils_RuniskovsDan::GetSurvivalistLocation(Component) };
	
	// --- Calculate Direction ---
	Steering.LinearVelocity = -(Target - Postion);
	
	return Steering;
}
