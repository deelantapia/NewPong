// Fill out your copyright notice in the Description page of Project Settings.

#include "NewPong.h"
#include "PongBallActor.h"

// Sets default values
APongBallActor::APongBallActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set Box Component physical attributes
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
	RootComponent = BoxComponent;
	BoxComponent->InitBoxExtent(FVector(100.0f, 100.0f, 100.0f));
	BoxComponent->SetCollisionProfileName(TEXT("PhysicsActor"));
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxComponent->SetSimulatePhysics(true);
	BoxComponent->SetEnableGravity(false);
	BoxComponent->SetNotifyRigidBodyCollision(true);
	BoxComponent->BodyInstance.bLockXTranslation = true;
	BoxComponent->SetLinearDamping(0.0f);
	
	// Create the visual box of the pong ball.
	BoxVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	BoxVisual->AttachTo(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));

	if (BoxVisualAsset.Succeeded()) {
		BoxVisual->SetStaticMesh(BoxVisualAsset.Object);
		BoxVisual->SetWorldScale3D(FVector(2.0f));
	}

	// Apply the pong ball material.
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> Material(TEXT("Material'/Game/StarterContent/Materials/M_PongBall_Inst.M_PongBall_Inst'"));

	if (Material.Object != NULL) {
		BoxVisual->SetMaterial(0, Material.Object);
		BoxVisual->SetRelativeLocation(FVector(0.0f, 0.0f, -100.0f));
	}

	// Delegate what happends when the ball interacts with other objects.	
	BoxComponent->OnComponentHit.AddDynamic(this, &APongBallActor::OnPongActorHit);
	this->OnActorBeginOverlap.AddDynamic(this, &APongBallActor::OutOfBounds);

	PongBallSpeedMap.Add("Easy", 1750.0f);
	PongBallSpeedMap.Add("Medium", 2187.5f);
	PongBallSpeedMap.Add("Hard", 2625.0f);

	BoxCompSizeMap.Add("Easy", FVector(100.0f, 100.0f, 100.0f));
	BoxCompSizeMap.Add("Medium", FVector(75.0f, 75.0f, 75.0f));
	BoxCompSizeMap.Add("Hard", FVector(50.0f, 50.0f, 50.0f));

	BoxVisualSizeMap.Add("Easy", FVector(2.0f));
	BoxVisualSizeMap.Add("Medium", FVector(1.5f));
	BoxVisualSizeMap.Add("Hard", FVector(1.0f));

	BoxVisRelLocMap.Add("Easy", FVector(0.0f, 0.0f, -100.0f));
	BoxVisRelLocMap.Add("Medium", FVector(0.0f, 0.0f, -75.0f));
	BoxVisRelLocMap.Add("Hard", FVector(0.0f, 0.0f, -50.0f));
}

// Called when the game starts or when spawned
void APongBallActor::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle UniqueHandle;
	Magnitude = 1750.0f;

	SetActorLocation(FVector(0.0f, 0.0f, 1500.0f));
	BoxComponent->SetPhysicsAngularVelocity(FVector(250.0f, 250.0f, 250.0f));
	GetWorldTimerManager().SetTimer(UniqueHandle, this, &APongBallActor::SetInitialVelocity, 3.0f, false);
}

// Called every frame
void APongBallActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	/*Trying to account for when pong ball hits one of the paddles and converts the linear velocity to angular velocity.
	There should be a loss in the X velocity. If there is I want to use my previous y velocity to maintain the pong ball speed so it maintains the pace
	the players are used to seeing the pong ball move as it was set in the BeginPlay function*/
	FVector NewLocation = GetActorLocation() + (CurrentLinearVelocity * DeltaTime);
	if (abs((long)CurrentLinearVelocity.Y) != abs((long)PreviousLinearVelocity.Y)) {
		CurrentLinearVelocity.Y = PreviousLinearVelocity.Y;
	}

	if (abs((long)CurrentLinearVelocity.Z) != abs((long)PreviousLinearVelocity.Z)) {
		CurrentLinearVelocity.Z = PreviousLinearVelocity.Z;
	}

	SetActorLocation(NewLocation);
	PreviousLinearVelocity = CurrentLinearVelocity;
}

void APongBallActor::OnPongActorHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	if (!(BoxComponent->GetPhysicsLinearVelocity().Equals(FVector().ZeroVector))) {
		BoxComponent->SetPhysicsLinearVelocity(FVector().ZeroVector);
	}
	if (OtherActor != NULL) {
		FVector bounceVector = CurrentLinearVelocity;
		APongPaddlePawn* CollidedPaddle = Cast<APongPaddlePawn>(OtherActor);
		if (CollidedPaddle != NULL) {
			FVector PaddleVector = CollidedPaddle->GetCurrentLinearVelocity();

			if (PaddleVector.Z < 0.0f) {
				/*if the other actor had z speed then set the z speed to the pong actor's linear velocity*/
				bounceVector.Z = -1.0f * Magnitude;
			}
			else if (PaddleVector.Z > 0.0f) {
				bounceVector.Z = Magnitude;
			}

			bounceVector.Y *= -1.0f;
			CurrentLinearVelocity = bounceVector;
		}
		else {
			ABlockingVolume* OtherVolume = Cast<ABlockingVolume>(OtherActor);
			if (OtherVolume != NULL) {
				bounceVector.Z *= -1.0f;
				CurrentLinearVelocity = bounceVector;
			}
		}
	}
}

void APongBallActor::OutOfBounds(class AActor* OtherActor) {
	if (OtherActor != NULL) {
		ASideBoundBoxActor* ASideBoundBox = Cast<ASideBoundBoxActor>(OtherActor);
		if (ASideBoundBox != NULL) {
			FTimerHandle UniqueHandle;
			CurrentLinearVelocity = PreviousLinearVelocity = FVector().ZeroVector;
			BoxComponent->SetPhysicsLinearVelocity(FVector().ZeroVector);
			SetActorLocation(FVector(0.0f, 0.0f, 1500.0f));
			GetWorldTimerManager().SetTimer(UniqueHandle, this, &APongBallActor::SetInitialVelocity, 3.0f, false);
		}
	}
}

void APongBallActor::SetInitialVelocity() {
	float YValue = FMath().RandRange(0, 1);
	float ZValue = FMath().RandRange(0, 1);

	YValue = YValue > 0.5 ? 1.0f : -1.0f;
	ZValue = ZValue > 0.5 ? 1.0f : -1.0f;
	SetVelocity(Magnitude * FVector(0.0f, YValue, ZValue));
}

void APongBallActor::SetVelocity(FVector NewVelocity) {
	CurrentLinearVelocity = PreviousLinearVelocity = NewVelocity;
}

void APongBallActor::SetDifficulty(FString PongDifficulty) {
	BoxComponent->SetBoxExtent(*BoxCompSizeMap.Find(PongDifficulty));
	BoxVisual->SetWorldScale3D(*BoxVisualSizeMap.Find(PongDifficulty));
	BoxVisual->SetRelativeLocation(*BoxVisRelLocMap.Find(PongDifficulty));
	Magnitude = *PongBallSpeedMap.Find(PongDifficulty);
}