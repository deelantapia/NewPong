// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SideBoundBoxActor.h"
#include "PongPaddlePawn.h"
#include "PongBallActor.generated.h"

UCLASS()
class NEWPONG_API APongBallActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APongBallActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY()
		UStaticMeshComponent* BoxVisual;

	UPROPERTY(EditAnywhere)
		UBoxComponent* BoxComponent;

	UPROPERTY()
		FVector PreviousLinearVelocity;

	UPROPERTY()
		FVector CurrentLinearVelocity;

	UPROPERTY()
		float Magnitude;

	UFUNCTION()
		void OutOfBounds(class AActor* OtherActor);

	UFUNCTION()
		void OnPongActorHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
		void SetInitialVelocity();

	UFUNCTION()
		void SetVelocity(FVector NewVelocity);

	void SetDifficulty(FString PongDifficulty);

protected:
	TMap<FString, float> PongBallSpeedMap;
	TMap<FString, FVector> BoxCompSizeMap;
	TMap<FString, FVector> BoxVisualSizeMap;
	TMap<FString, FVector> BoxVisRelLocMap;
};
