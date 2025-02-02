// Fill out your copyright notice in the Description page of Project Settings.


#include "ObstacleSpawner.h"
#include "BountyDash.h"
#include "BountyDashGameModeBase.h"
#include "EngineUtils.h"
#include "Engine/TargetPoint.h"
#include "Components/SphereComponent.h"
#include "Floor.h"
#include "Obstacle.h"

// Sets default values
AObstacleSpawner::AObstacleSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	check(Scene);
	RootComponent = Scene;

	SpawnTimer = 1.5f;
}

// Called when the game starts or when spawned
void AObstacleSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	for (TActorIterator<ATargetPoint> TargetIter(GetWorld()); TargetIter; ++TargetIter)
	{
		SpawnTargets.Add(*TargetIter);
	}

	for (TActorIterator<AFloor> FloorIter(GetWorld()); FloorIter; ++FloorIter)
	{
		if (FloorIter->GetWorld() == GetWorld())
		{
			KillPoint = FloorIter->GetKillPoint();
			SpawnPoint = FloorIter->GetSpawnPoint();
		}
	}

	TimeSinceLastSpawn = SpawnTimer;
}

// Called every frame
void AObstacleSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeSinceLastSpawn += DeltaTime;

	float trueSpawnTime = SpawnTimer / GetCustomGameMode<ABountyDashGameModeBase>(GetWorld())->GetGameLevel();

	if (TimeSinceLastSpawn > trueSpawnTime)
	{
		TimeSinceLastSpawn = 0.0f;
		SpawnObstacle();
	}
}

void AObstacleSpawner::SpawnObstacle()
{
	if (SpawnTargets.Num() > 0 && ObstaclesToSpawn.Num() > 0)
	{
		short Spawner = FMath::Rand() % SpawnTargets.Num();
		short Obstacle = FMath::Rand() % ObstaclesToSpawn.Num();
		float CapsuleOffset = 0.0f;

		FActorSpawnParameters SpawnInfo;

		FTransform transform = SpawnTargets[Spawner]->GetTransform();
		transform.SetLocation(FVector(SpawnPoint, transform.GetLocation().Y, transform.GetLocation().Z));

		AObstacle* newObs = GetWorld()->SpawnActor<AObstacle>(ObstaclesToSpawn[Obstacle], transform, SpawnInfo);

		if (newObs)
		{
			newObs->SetKillPoint(KillPoint);

			USphereComponent* obsSphere = Cast<USphereComponent>(newObs->GetComponentByClass(USphereComponent::StaticClass()));

			if (obsSphere)
			{
				newObs->AddActorLocalOffset(FVector(0.0f, 0.0f, obsSphere->GetUnscaledSphereRadius()));
			}
		}
	}
}