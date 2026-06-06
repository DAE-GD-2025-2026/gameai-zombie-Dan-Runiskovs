#include "Steering/FSteeringBehaviorBase_RuniskovsDan.h"

#include "Components/SteeringComponent_RuniskovsDan.h"
#include "Tasks/Utils_RuniskovsDan.h"

FSteeringOutput FSeek_RuniskovsDan::CalculateSteering(float DeltaT, USteeringComponent_RuniskovsDan& Component)
{
	FSteeringOutput Steering{};

	// --- Get Actor Position ---
	const auto Position{ MyBTTUtils_RuniskovsDan::GetSurvivalistLocation(Component) };
	
	// --- Calculate Linear Velocity ---
	Steering.LinearVelocity = Target - Position;
	
	// --- Too close? ---
	if (constexpr auto CloseDstSquared{25.f}; Steering.LinearVelocity.SquaredLength() <= CloseDstSquared)
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
	const auto Position{ MyBTTUtils_RuniskovsDan::GetSurvivalistLocation(Component) };
	
	// --- Calculate Linear Velocity ---
	Steering.LinearVelocity = -(Target - Position);
	
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
	const auto* Survivalist{ Component.GetOwner() };
	verify(Survivalist);
	const FVector2D ForwardVector{ Survivalist->GetActorForwardVector().X , Survivalist->GetActorForwardVector().Y };

	// --- Save the angle --- 
	LastAngleDeg = NewAngleDeg;
	
	// --- Set new target ---
	const auto AgentLocation{ Survivalist->GetActorLocation() };
	const FVector2D NewTargetLocation{ 
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
	const auto Position{ MyBTTUtils_RuniskovsDan::GetSurvivalistLocation(Component) };
	
	// --- Calculate Direction ---
	Steering.LinearVelocity = -(Target - Position);
	
	return Steering;
}
