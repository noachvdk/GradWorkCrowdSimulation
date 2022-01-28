// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Spawner.h"

#include "Kismet/KismetMathLibrary.h"

// Sets default values
ACPP_Spawner::ACPP_Spawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnVolume"));
	BoxCollision->SetRelativeLocation(FVector(0, 0, 0));
	BoxCollision->SetGenerateOverlapEvents(false);
	BoxCollision->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACPP_Spawner::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle SpawnHandle{};
	GetWorld()->GetTimerManager().SetTimer(SpawnHandle, this, &ACPP_Spawner::CheckForSpawn, SpawnTime, true, 0.1f);
}

void ACPP_Spawner::CheckForSpawn()
{
	//check if still need to spawn
	if(CurrentSpawnedAmount < SpawnMaxAmount)
	{
		//Get spawn location with bounds and rotate randomly
		const FVector SpawnLocation = UKismetMathLibrary::RandomPointInBoundingBox(BoxCollision->GetComponentLocation(), BoxCollision->GetScaledBoxExtent());
		//GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Red, FString::Printf(TEXT("Need to spawn")));
		SpawnPedestrian(SpawnLocation);
	}
}
