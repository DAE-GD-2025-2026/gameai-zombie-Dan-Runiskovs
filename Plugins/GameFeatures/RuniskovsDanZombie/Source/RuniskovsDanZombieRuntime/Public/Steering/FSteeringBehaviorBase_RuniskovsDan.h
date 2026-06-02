#pragma once

class USteeringComponent_RuniskovsDan;

struct FSteeringOutput
{
	FVector LinearVelocity;
};

using FTargetData = FVector;

class FSteeringBehaviorBase_RuniskovsDan
{
public:
	FSteeringBehaviorBase_RuniskovsDan() = default;
	virtual ~FSteeringBehaviorBase_RuniskovsDan() = default;

	// Override to implement your own behavior
	virtual FSteeringOutput CalculateSteering(float DeltaT, USteeringComponent_RuniskovsDan& Component) = 0;

	void SetTarget(const FTargetData& NewTarget) { Target = NewTarget; }
protected:
	FVector Target;
};

class FSeek_RuniskovsDan final : public FSteeringBehaviorBase_RuniskovsDan
{
public:
	FSeek_RuniskovsDan() = default;
	virtual  ~FSeek_RuniskovsDan() override = default;
	
	//Seek Behavior
	virtual FSteeringOutput CalculateSteering(float DeltaT, USteeringComponent_RuniskovsDan& Component) override;
};

