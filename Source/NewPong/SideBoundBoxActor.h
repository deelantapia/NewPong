// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SideBoundBoxActor.generated.h"

UCLASS()
class NEWPONG_API ASideBoundBoxActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASideBoundBoxActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

protected:

	UPROPERTY(EditAnywhere)
		UBoxComponent* BoxComponent;
	
};
