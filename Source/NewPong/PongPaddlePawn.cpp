// Fill out your copyright notice in the Description page of Project Settings.

#include "NewPong.h"
#include "PongBallActor.h"
#include "PongPaddlePawn.h"

const FVector APongPaddlePawn::EasyBoxCompSize = FVector(25.0f, 50.0f, 500.0f);
const FVector APongPaddlePawn::EasyBoxVisualSize = FVector(2.0f);

const FVector APongPaddlePawn::MediumBoxCompSize = FVector(25.0f, 50.0f, 375.0f);
const FVector APongPaddlePawn::MediumBoxVisualSize = FVector(2.0f, 2.0f, 1.5f);

const FVector APongPaddlePawn::HardBoxCompSize = FVector(25.0f, 50.0f, 250.0f);
const FVector APongPaddlePawn::HardBoxVisualSize = FVector(2.0f, 2.0f, 1.0f);

// Sets default values
APongPaddlePawn::APongPaddlePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
	RootComponent = BoxComponent;
	BoxComponent->InitBoxExtent(FVector(25.0f, 50.0f, 500.0f));
	BoxComponent->SetCollisionProfileName(TEXT("Pawn"));*/

	//BoxVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	//BoxVisual->AttachTo(RootComponent);

	BoxVisual = CreateDefaultSubobject<UDestructibleComponent>(TEXT("VisualRepresentation"));
	RootComponent = BoxVisual;
	BoxVisual->SetCollisionProfileName(TEXT("Pawn"));
	
	
	static ConstructorHelpers::FObjectFinder<UDestructibleMesh> BoxVisualAsset(TEXT("/Game/StarterContent/Shapes/PongPaddle_DM.PongPaddle_DM"));
	//static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxVisualAsset(TEXT("/Game/StarterContent/Shapes/PongPaddle.PongPaddle"));
	if (BoxVisualAsset.Succeeded()) {
		//BoxVisual->SetStaticMesh(BoxVisualAsset.Object);
		BoxVisual->SetDestructibleMesh(BoxVisualAsset.Object);
		BoxVisual->SetWorldScale3D(FVector(2.0f));
		//BoxVisual->SetSimulatePhysics(false);
	}

	/*static ConstructorHelpers::FObjectFinder<UMaterialInterface> Material(TEXT("Material'/Game/StarterContent/Materials/M_PongPaddle_Inst.M_PongPaddle_Inst'"));
	if (Material.Succeeded()) {
		BoxVisual->SetMaterial(0, Material.Object);
	}*/

	/*BoxComponent->SetSimulatePhysics(true);
	BoxComponent->SetLinearDamping(0.0f);
	BoxComponent->SetEnableGravity(false);
	BoxComponent->BodyInstance.bLockXRotation = true;
	BoxComponent->BodyInstance.bLockYRotation = true;
	BoxComponent->BodyInstance.bLockZRotation = true;
	BoxComponent->BodyInstance.bLockXTranslation = true;
	BoxComponent->BodyInstance.bLockYTranslation = true;*/

	
	BoxVisual->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxVisual->SetNotifyRigidBodyCollision(true);
	BoxVisual->SetSimulatePhysics(true);
	BoxVisual->SetLinearDamping(0.0f);
	BoxVisual->SetEnableGravity(false);
	BoxVisual->BodyInstance.bLockXRotation = true;
	BoxVisual->BodyInstance.bLockYRotation = true;
	BoxVisual->BodyInstance.bLockZRotation = true;
	BoxVisual->BodyInstance.bLockXTranslation = true;
	BoxVisual->BodyInstance.bLockYTranslation = true;

	BoxVisual->OnComponentHit.AddDynamic(this, &APongPaddlePawn::OnPaddleHit);
}

// Called when the game starts or when spawned
void APongPaddlePawn::BeginPlay()
{
	Super::BeginPlay();
	PlayerScore = 0;
}

// Called every frame
void APongPaddlePawn::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (!CurrentLinearVelocity.IsZero()) {
		FVector NewLocation = GetActorLocation() + (CurrentLinearVelocity * DeltaTime);
		SetActorLocation(NewLocation);
	}
	/*if (!(BoxComponent->GetPhysicsLinearVelocity().Equals(FVector().ZeroVector))) {
		BoxComponent->SetPhysicsLinearVelocity(FVector().ZeroVector);
	}*/
	if (!(BoxVisual->GetPhysicsLinearVelocity().Equals(FVector().ZeroVector))) {
		BoxVisual->SetPhysicsLinearVelocity(FVector().ZeroVector);
	}
}

// Called to bind functionality to input
void APongPaddlePawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

void APongPaddlePawn::SetSize(FString PongDifficulty) {
	if (PongDifficulty.Equals("Easy")) {
		//BoxComponent->SetBoxExtent(EasyBoxCompSize);
		BoxVisual->SetWorldScale3D(EasyBoxVisualSize);
	}
	else if (PongDifficulty.Equals("Medium")) {
		//BoxComponent->SetBoxExtent(MediumBoxCompSize);
		BoxVisual->SetWorldScale3D(MediumBoxVisualSize);
	}
	else if (PongDifficulty.Equals("Hard")) {
		//BoxComponent->SetBoxExtent(HardBoxCompSize);
		BoxVisual->SetWorldScale3D(HardBoxVisualSize);
	}
}

FVector APongPaddlePawn::GetCurrentLinearVelocity() {
	return CurrentLinearVelocity;
}

void APongPaddlePawn::SetCurrentLinearVelocity(FVector NewCurrentLinearVelocity) {
	CurrentLinearVelocity = NewCurrentLinearVelocity;
}

float APongPaddlePawn::GetHealth() {
	return health;
}

void APongPaddlePawn::SetHealth(float newHealthValue) {
	health = newHealthValue;
}

void APongPaddlePawn::ReduceHealth(float DamageValue) {
	health -= DamageValue;
}

void APongPaddlePawn::OnPaddleHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) {
	if (OtherActor != NULL) {
		APongBallActor* PongBall = Cast<APongBallActor>(OtherActor);
		if (PongBall != NULL) {
			if (bIsInSurvivalMode) {
				ReduceHealth(0.20f);
				if (GetHealth() < 0.20f) {
					BoxVisual->ApplyDamage(1.0f, PongBall->GetActorLocation(), FVector(0.0f, 1.0f, 1.0f), 100.0f);
				}
			}
		}
	}
	else if(OtherComp != NULL) {

	}
}

void APongPaddlePawn::SetSurvivalMode(bool bSurvivalMode) {
	bIsInSurvivalMode = bSurvivalMode;
}

int32 APongPaddlePawn::GetPlayerScore()
{
	return PlayerScore;
}

void APongPaddlePawn::ResetPlayerScore() {
	PlayerScore = 0;
}
