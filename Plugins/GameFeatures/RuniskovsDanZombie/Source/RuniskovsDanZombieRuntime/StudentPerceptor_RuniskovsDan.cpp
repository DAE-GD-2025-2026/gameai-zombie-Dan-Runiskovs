// Fill out your copyright notice in the Description page of Project Settings.


#include "StudentPerceptor_RuniskovsDan.h"


UStudentPerceptor_RuniskovsDan::UStudentPerceptor_RuniskovsDan()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UStudentPerceptor_RuniskovsDan::BeginPlay()
{
	Super::BeginPlay();
	
	if (auto PerceptionComp = GetOwner()->GetComponentByClass<UAIPerceptionComponent>())
	{
		PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &UStudentPerceptor_RuniskovsDan::OnPerceptionUpdated);
	}
}

void UStudentPerceptor_RuniskovsDan::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	GEngine->AddOnScreenDebugMessage(5, 1.f, FColor::Green, 
	FString::Printf(TEXT("Saw Something!")));
}
