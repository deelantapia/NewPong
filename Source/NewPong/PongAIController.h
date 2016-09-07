// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "PongBallActor.h"
#include "PongPaddlePawn.h"
#include "PongAIController.generated.h"

/**
 * 
 */
UCLASS()
class NEWPONG_API APongAIController : public AAIController
{
	GENERATED_BODY()
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

public:

	UPROPERTY()
		APongBallActor* PongBall;

	UPROPERTY()
		APongPaddlePawn*	AIPawn;

	virtual void Possess(APawn* pawn) override;
	virtual void UnPossess() override;	
};
