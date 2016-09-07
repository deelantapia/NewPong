// Fill out your copyright notice in the Description page of Project Settings.

#include "NewPong.h"
#include "PongPlayerController.h"

APongPlayerController::APongPlayerController() {

}

void APongPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();
	InputComponent->BindAxis("Move P1 Z", this, &APongPlayerController::Move_P1_ZAxis);
	InputComponent->BindAxis("Move P2 Z", this, &APongPlayerController::Move_P2_ZAxis);
}

void APongPlayerController::Move_P1_ZAxis(float AxisValue) {
	// Move at 100 units per second up or down
	if (PlayerOnePawn != NULL) {
		FVector P1Velocity = PlayerOnePawn->GetCurrentLinearVelocity();
		P1Velocity.Z = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 1000.0f;
		PlayerOnePawn->SetCurrentLinearVelocity(P1Velocity);
	}
}

void APongPlayerController::Move_P2_ZAxis(float AxisValue) {
	if (PlayerTwoPawn != NULL) {
		// Move at 100 units per second up or down
		FVector P2Velocity = PlayerTwoPawn->GetCurrentLinearVelocity();
		P2Velocity.Z = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 1000.0f;
		PlayerTwoPawn->SetCurrentLinearVelocity(P2Velocity);
	}
}

void APongPlayerController::SetPlayerOnePawn(APongPaddlePawn* POnePawn) {
	PlayerOnePawn = POnePawn;
}

void APongPlayerController::SetPlayerTwoPawn(APongPaddlePawn* PTwoPawn) {
	PlayerTwoPawn = PTwoPawn;
}

bool APongPlayerController::HasTwoPlayers() {
	return PlayerTwoPawn != NULL;
}
