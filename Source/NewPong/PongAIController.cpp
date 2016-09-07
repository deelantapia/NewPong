// Fill out your copyright notice in the Description page of Project Settings.

#include "NewPong.h"
#include "PongAIController.h"

// Called when the game starts or when spawned
void APongAIController::BeginPlay()
{
	Super::BeginPlay();
	for (TObjectIterator<AActor> Itr; Itr; ++Itr) {
		if (Itr->IsA(APongBallActor::StaticClass())) {
			PongBall = Cast<APongBallActor>(*Itr);
		}
	}
}

// Called every frame
void APongAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (AIPawn != NULL) {
		if (PongBall != NULL) {
			if (AIPawn->GetActorLocation().Z < PongBall->GetActorLocation().Z) {
				AIPawn->SetCurrentLinearVelocity(FVector(0.0f, 0.0f, 1000.0f));
			}
			else if (AIPawn->GetActorLocation().Z > PongBall->GetActorLocation().Z) {
				AIPawn->SetCurrentLinearVelocity(FVector(0.0f, 0.0f, -1000.0f));
			}
			else {
				AIPawn->SetCurrentLinearVelocity(FVector(0.0f, 0.0f, 0.0f));
			}
		}

		FVector NewLocation = AIPawn->GetActorLocation() + (AIPawn->GetCurrentLinearVelocity() * DeltaTime);
		AIPawn->SetActorLocation(NewLocation);
	}
}

void APongAIController::Possess(APawn* Pawn) {
	Super::Possess(Pawn);
	AIPawn = Cast<APongPaddlePawn>(GetPawn());
}

void APongAIController::UnPossess() {
	Super::UnPossess();
	AIPawn = NULL;
}


