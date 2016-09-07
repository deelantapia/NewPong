// Fill out your copyright notice in the Description page of Project Settings.

#include "NewPong.h"
#include "NewPongGameMode.h"

void ANewPongGameMode::BeginPlay() {
	Super::BeginPlay();

	DefaultPawnClass = APongPaddlePawn::StaticClass();

	// Create Spawning Info
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;
	FVector LeftPawnLocation = FVector(0, 1750, 1500);
	FVector RightPawnLocation = FVector(0, -1750, 1500);
	FRotator SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);

	// Spawn Side Bound Boxes
	FVector LeftBoxLoc = FVector(0.0f, 2250.0f, 2000.0f);
	FVector RightBoxLoc = FVector(0.0f, -2250.0f, 2000.0f);
	LeftSideBoundBox = GetWorld()->SpawnActor<ASideBoundBoxActor>(ASideBoundBoxActor::StaticClass(), LeftBoxLoc, SpawnRotation, SpawnParams);
	RightSideBoundBox = GetWorld()->SpawnActor<ASideBoundBoxActor>(ASideBoundBoxActor::StaticClass(), RightBoxLoc, SpawnRotation, SpawnParams);

	// Spawn Player Pawns
	FirstPlayerPawn = GetWorld()->SpawnActor<APongPaddlePawn>(DefaultPawnClass, LeftPawnLocation, SpawnRotation, SpawnParams);
	SecondPlayerPawn = GetWorld()->SpawnActor<APongPaddlePawn>(DefaultPawnClass, RightPawnLocation, SpawnRotation, SpawnParams);

	// Auto set game to player vs AI
	EnemyPlayer = GetWorld()->SpawnActor<APongAIController>(APongAIController::StaticClass(), SpawnParams);
	EnemyPlayer->Possess(SecondPlayerPawn);

	//Spawn Pong ball
	FVector PongBallLocation = FVector(0.0f, 0.0f, 1500.0f);
	PongBall = GetWorld()->SpawnActor<APongBallActor>(APongBallActor::StaticClass(), PongBallLocation, SpawnRotation, SpawnParams);

	if (MenuWidgetClass != NULL) {
		MyPlayer = Cast<APongPlayerController>(GetWorld()->GetFirstPlayerController());
		PauseGame();
		MyPlayer->SetPlayerOnePawn(FirstPlayerPawn);
	}
}

void ANewPongGameMode::NewPongGame() {
	FirstPlayerPawn->SetActorHiddenInGame(false);
	SecondPlayerPawn->SetActorHiddenInGame(false);
	PongBall->SetActorHiddenInGame(false);
}

void ANewPongGameMode::EndPongGame() {
	FirstPlayerPawn->SetActorHiddenInGame(true);
	FirstPlayerPawn->ResetPlayerScore();
	SecondPlayerPawn->SetActorHiddenInGame(true);
	SecondPlayerPawn->ResetPlayerScore();
	PongBall->SetActorHiddenInGame(true);
	SetVersusMode(1);
	ChangeToOriginalMode();
}

void ANewPongGameMode::Tick(float DeltaSeconds) {
	/*if (LeftSideBoundBox->GetPlayerScore() > 9 || RightSideBoundBox->GetPlayerScore() > 9) {
		GameOver();
	}*/
}

void ANewPongGameMode::SetMenuWidgetClass(TSubclassOf<UUserWidget> widget) {
	if (CurrentWidget != nullptr) {
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}

	MenuWidgetClass = widget;

	CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), MenuWidgetClass);
	if (CurrentWidget != nullptr) {
		CurrentWidget->AddToViewport();
	}
}

void ANewPongGameMode::PlayGame() {
	if (CurrentWidget != nullptr) {
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}

	if (MyPlayer != nullptr) {
		//MyPlayer->bShowMouseCursor = false;
		MyPlayer->SetPause(false);
		FInputModeGameOnly Mode;
		MyPlayer->SetInputMode(Mode);
	}
}

void ANewPongGameMode::PauseGame() {
	if (MyPlayer != nullptr) {
		MyPlayer->SetPause(true);
		FInputModeUIOnly Mode;
		MyPlayer->SetInputMode(Mode);
		MyPlayer->bShowMouseCursor = true;
	}
}

void ANewPongGameMode::GameOver() {
	PauseGame();
	SetMenuWidgetClass(GameOverMenuWidget);
}

void ANewPongGameMode::SetDifficulty(FString PongDifficulty) {
	PongBall->SetDifficulty(PongDifficulty);
	FirstPlayerPawn->SetSize(PongDifficulty);
	SecondPlayerPawn->SetSize(PongDifficulty);
}

void ANewPongGameMode::SetVersusMode(int32 NumberHumanPlayers) {
	if (NumberHumanPlayers < 2) {
		// have AI possess right pawn
		MyPlayer->SetPlayerTwoPawn(NULL);
		EnemyPlayer->Possess(SecondPlayerPawn);
	}
	else {
		// have PongPlayerController control the right pawn
		EnemyPlayer->UnPossess();
		MyPlayer->SetPlayerTwoPawn(SecondPlayerPawn);
	}
}

bool ANewPongGameMode::IsATwoPlayerGame() {
	return MyPlayer->HasTwoPlayers();
}

void ANewPongGameMode::PassInputToPlayer(int32 PlayerNumber, float AxisDirection) {
	if (PlayerNumber == 1) {
		MyPlayer->Move_P1_ZAxis(AxisDirection);
	}
	else {
		MyPlayer->Move_P2_ZAxis(AxisDirection);
	}
}

void ANewPongGameMode::ChangeToSurvivalMode() {
	FirstPlayerPawn->SetHealth(1.0f);
	FirstPlayerPawn->SetSurvivalMode(true);
	SecondPlayerPawn->SetHealth(1.0f);	
	SecondPlayerPawn->SetSurvivalMode(true);
	SetVersusMode(2);
	SetSurvivalModeFlag(true);
}

void ANewPongGameMode::ChangeToOriginalMode() {
	FirstPlayerPawn->SetSurvivalMode(false);
	SecondPlayerPawn->SetSurvivalMode(false);
	SetSurvivalModeFlag(false);
}

float ANewPongGameMode::GetFirstPlayerHealth() {
	return FirstPlayerPawn->GetHealth();
}

float ANewPongGameMode::GetSecondPlayerHealth() {
	return SecondPlayerPawn->GetHealth();
}

int32 ANewPongGameMode::GetFirstPlayerScore() {
	return FirstPlayerPawn->GetPlayerScore();
}

int32 ANewPongGameMode::GetSecondPlayerScore() {
	return SecondPlayerPawn->GetPlayerScore();
}

bool ANewPongGameMode::GetSurvivalModeFlag() {
	return bSurvivalModeFlag;
}

void ANewPongGameMode::SetSurvivalModeFlag(bool bNewFlagValue) {
	bSurvivalModeFlag = bNewFlagValue;
}

void ANewPongGameMode::FromPauseToMainMenu() {

}
