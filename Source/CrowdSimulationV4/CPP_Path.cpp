// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Path.h"

// Sets default values
ACPP_Path::ACPP_Path()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	Spline->SetRelativeLocation(FVector(0, 0, 0));
	Spline->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACPP_Path::BeginPlay()
{
	Super::BeginPlay();
}


void ACPP_Path::PlacePathPoints()
{
	//Can't use this unless i can set the correct collision in the cpp class of the path point.

	//Reset variables
	Distance = 0;

	for (ACPP_PathPoint* Point : PathPoints)
		Point->SetLifeSpan(0.0f);
	PathPoints.Empty();
}
