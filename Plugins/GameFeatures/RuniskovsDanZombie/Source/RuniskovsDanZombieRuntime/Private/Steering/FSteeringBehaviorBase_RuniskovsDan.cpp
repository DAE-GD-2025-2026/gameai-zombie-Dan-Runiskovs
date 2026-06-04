#include "Steering/FSteeringBehaviorBase_RuniskovsDan.h"

#include "Steering/SteeringComponent_RuniskovsDan.h"

FSteeringOutput FSeek_RuniskovsDan::CalculateSteering(float DeltaT, USteeringComponent_RuniskovsDan& Component)
{
	FSteeringOutput Steering{};
	constexpr float CloseDstSquared{25.f};
	
	Steering.LinearVelocity = Target - Component.GetOwner()->GetActorLocation();
	
	if (Steering.LinearVelocity.SquaredLength() <= CloseDstSquared)
		Steering.LinearVelocity = FVector::ZeroVector;	
	
	if (!Steering.LinearVelocity.IsNearlyZero())
		Steering.LinearVelocity.Normalize();
	
	return Steering;
}

FSteeringOutput FFlee_RuniskovsDan::CalculateSteering(float DeltaT, USteeringComponent_RuniskovsDan& Component)
{
	FSteeringOutput Steering{};
	
	Steering.LinearVelocity = -(Target - Component.GetOwner()->GetActorLocation());
	
	if (!Steering.LinearVelocity.IsNearlyZero())
		Steering.LinearVelocity.Normalize();
	
	return Steering; 
}
