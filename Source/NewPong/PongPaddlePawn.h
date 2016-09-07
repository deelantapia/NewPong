// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "Engine/DestructibleMesh.h"
#include "PongPaddlePawn.generated.h"

UCLASS()
class NEWPONG_API APongPaddlePawn : public APawn
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
		UBoxComponent* BoxComponent;

	// Sets default values for this pawn's properties
	APongPaddlePawn();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	void SetSize(FString PongDifficulty);

	FVector GetCurrentLinearVelocity();

	void SetCurrentLinearVelocity(FVector NewCurrentVelocity);

	float GetHealth();

	void SetHealth(float newHealthValue);

	void ReduceHealth(float DamageValue);

	void SetSurvivalMode(bool bSurvivalMode);

	int32 GetPlayerScore();

	void ResetPlayerScore();

	/*UStaticMeshComponent* BoxVisual;*/

protected:

	UPROPERTY()
		UDestructibleComponent* BoxVisual;

	// Input variables
	FVector CurrentLinearVelocity;

	float health;
	int32 PlayerScore;
	bool bIsInSurvivalMode;

	static const FVector EasyBoxCompSize;
	static const FVector EasyBoxVisualSize;

	static const FVector MediumBoxCompSize;
	static const FVector MediumBoxVisualSize;

	static const FVector HardBoxCompSize;
	static const FVector HardBoxVisualSize;

	UFUNCTION()
		void OnPaddleHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	
};
