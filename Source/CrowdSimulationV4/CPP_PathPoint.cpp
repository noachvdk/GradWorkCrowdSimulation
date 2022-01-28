// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_PathPoint.h"

// Sets default values
ACPP_PathPoint::ACPP_PathPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BaseCollision"));
	BoxCollision->SetRelativeLocation(FVector(0, 0, 0));
	BoxCollision->SetGenerateOverlapEvents(true);
	BoxCollision->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACPP_PathPoint::BeginPlay()
{
	Super::BeginPlay();
	
}


