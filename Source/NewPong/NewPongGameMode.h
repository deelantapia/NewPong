// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "GameFramework/GameMode.h"
#include "PongPaddlePawn.h"
#include "PongBallActor.h"
#include "PongPlayerController.h"
#include "PongAIController.h"
#include "SideBoundBoxActor.h"
#include "NewPongGameMode.generated.h"

/**
 * 
 */
UCLASS()
class NEWPONG_API ANewPongGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	/*	Called when the game starts	*/
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	/*	Remove the current menu widget and create a new one form the specified class, if provided.	*/
	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		void PlayGame();

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		void PauseGame();

	void GameOver();

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		void SetMenuWidgetClass(TSubclassOf<UUserWidget> widget);

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		void SetDifficulty(FString PongDifficulty);

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		void SetVersusMode(int32 NumberHumanPlayers);

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
		bool IsATwoPlayerGame();

	UFUNCTION(BlueprintCallable, Category = "UMG GAME")
		void PassInputToPlayer(int32 PlayerNumber, float AxisDirection);

	UFUNCTION(BlueprintCallable, Category = "UMG GAME")
		void ChangeToSurvivalMode();

	UFUNCTION(BlueprintCallable, Category = "UMG GAME")
		void ChangeToOriginalMode();

	UFUNCTION(BlueprintCallable, Category = "UMG GAME")
		float GetFirstPlayerHealth();

	UFUNCTION(BlueprintCallable, Category = "UMG GAME")
		float GetSecondPlayerHealth();

	UFUNCTION(BlueprintCallable, Category = "UMG GAME")
		bool GetSurvivalModeFlag();

	UFUNCTION()
		void SetSurvivalModeFlag(bool bNewFlagValue);

	UFUNCTION(BlueprintCallable, Category = "UMG GAME")
		void NewPongGame();

	UFUNCTION(BlueprintCallable, Category = "UMG GAME")
		void EndPongGame();
	
	UFUNCTION(BlueprintCallable, Category = "UMG GAME")
		int32 GetFirstPlayerScore();

	UFUNCTION(BlueprintCallable, Category = "UMG GAME")
		int32 GetSecondPlayerScore();

	UFUNCTION(BlueprintCallable, Category = "UMG GAME")
		void FromPauseToMainMenu();

protected:
	/*	The widget class we will use as our menu when the game starts.	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
		TSubclassOf<UUserWidget> MenuWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
		TSubclassOf<UUserWidget> GameOverMenuWidget;

	/*	The widget instance that we are using as our menu	*/
	UPROPERTY()
		UUserWidget* CurrentWidget;

	UPROPERTY()
		APongPlayerController* MyPlayer;

	UPROPERTY()
		APongAIController* EnemyPlayer;

	UPROPERTY()
		APongBallActor* PongBall;

	UPROPERTY()
		APongPaddlePawn* FirstPlayerPawn;

	UPROPERTY()
		APongPaddlePawn* SecondPlayerPawn;

	UPROPERTY()
		ASideBoundBoxActor* LeftSideBoundBox;

	UPROPERTY()
		ASideBoundBoxActor* RightSideBoundBox;

	UPROPERTY()
		bool bSurvivalModeFlag;
	
	
};
