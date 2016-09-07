// Fill out your copyright notice in the Description page of Project Settings.

#include "NewPong.h"
#include "SideBoundBoxActor.h"


// Sets default values
ASideBoundBoxActor::ASideBoundBoxActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
	RootComponent = BoxComponent;
	BoxComponent->InitBoxExtent(FVector(50.0f, 25.0f, 2000.0f));
	BoxComponent->SetCollisionProfileName(TEXT("Trigger"));
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxComponent->SetSimulatePhysics(true);
	BoxComponent->SetEnableGravity(false);
}

// Called when the game starts or when spawned
void ASideBoundBoxActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASideBoundBoxActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}