#pragma once

class USteeringComponent_RuniskovsDan;

struct FSteeringOutput
{
	FVector2D LinearVelocity;
};

using FTargetData = FVector2D;

class FSteeringBehaviorBase_RuniskovsDan
{
public:
	FSteeringBehaviorBase_RuniskovsDan() = default;
	virtual ~FSteeringBehaviorBase_RuniskovsDan() = default;

	// Override to implement your own behavior
	virtual FSteeringOutput CalculateSteering(float DeltaT, USteeringComponent_RuniskovsDan& Component) = 0;

	void SetTarget(const FTargetData& NewTarget) { Target = NewTarget; }
	
	template<class T, std::enable_if_t<std::is_base_of_v<FSteeringBehaviorBase_RuniskovsDan, T>>* = nullptr>
	T* As() noexcept { return static_cast<T*>(this); }
protected:
	FVector2D Target;
};

class FSeek_RuniskovsDan : public FSteeringBehaviorBase_RuniskovsDan
{
public:
	FSeek_RuniskovsDan() = default;
	virtual  ~FSeek_RuniskovsDan() override = default;
	
	//Seek Behavior
	virtual FSteeringOutput CalculateSteering(float DeltaT, USteeringComponent_RuniskovsDan& Component) override;
};

class FFlee_RuniskovsDan final : public FSteeringBehaviorBase_RuniskovsDan
{
public:
	FFlee_RuniskovsDan() = default;
	virtual  ~FFlee_RuniskovsDan() override = default;
	
	// Flee Behavior
	virtual FSteeringOutput CalculateSteering(float DeltaT, USteeringComponent_RuniskovsDan& Component) override;
};

class FWander_RuniskovsDan final : public FSeek_RuniskovsDan
{
public:
	FWander_RuniskovsDan() = default;
	virtual  ~FWander_RuniskovsDan() override = default;
	
	// Wander Behaviour
	virtual FSteeringOutput CalculateSteering(float DeltaT, USteeringComponent_RuniskovsDan& Component) override;
private:
	const float TargetCircleRadius{ 200.f };
	const float TargetCircleOffset{ 400.f };
	const float	MaxOffset{ 40.f };
	float LastAngleDeg{};
};

class FLookAt_RuniskovsDan final : public FSteeringBehaviorBase_RuniskovsDan
{
	public:
	FLookAt_RuniskovsDan() = default;
	virtual  ~FLookAt_RuniskovsDan() override = default;
	
	// LookAt Behavior
	virtual FSteeringOutput CalculateSteering(float DeltaT, USteeringComponent_RuniskovsDan& Component) override; 
};

