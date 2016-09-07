// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "PongPaddlePawn.h"
#include "PongPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class NEWPONG_API APongPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	void SetPlayerOnePawn(APongPaddlePawn* POnePawn);
	void SetPlayerTwoPawn(APongPaddlePawn* PTwoPawn);

	// Input variables
	FVector CurrentVelocity;

	UPROPERTY()
		APongPaddlePawn* PlayerOnePawn;

	UPROPERTY()
		APongPaddlePawn* PlayerTwoPawn;

	bool HasTwoPlayers();

	// Input
	void Move_P1_ZAxis(float AxisValue);
	void Move_P2_ZAxis(float AxisValue);

protected:

	APongPlayerController();

	virtual void SetupInputComponent() override;

		
};
