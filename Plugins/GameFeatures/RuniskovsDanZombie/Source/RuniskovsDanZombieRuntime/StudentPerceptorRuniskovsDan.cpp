// Fill out your copyright notice in the Description page of Project Settings.


#include "StudentPerceptorRuniskovsDan.h"


UStudentPerceptorRuniskovsDan::UStudentPerceptorRuniskovsDan()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UStudentPerceptorRuniskovsDan::BeginPlay()
{
	Super::BeginPlay();
	
	if (auto PerceptionComp = GetOwner()->GetComponentByClass<UAIPerceptionComponent>())
	{
		PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &UStudentPerceptorRuniskovsDan::OnPerceptionUpdated);
	}
}

void UStudentPerceptorRuniskovsDan::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	GEngine->AddOnScreenDebugMessage(5, 1.f, FColor::Green, 
	FString::Printf(TEXT("Saw Something!")));
}
