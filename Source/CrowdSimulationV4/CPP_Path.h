// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "CPP_PathPoint.h"
#include "CPP_Path.generated.h"

UCLASS()
class CROWDSIMULATIONV4_API ACPP_Path : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_Path();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void PlacePathPoints();

	//Variables
	float Distance = 0.0f;
	float Interval = 400.0f;
	TArray<ACPP_PathPoint*> PathPoints{};

public:	
	UPROPERTY(Category = "SplineComponent", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		USplineComponent* Spline = nullptr;

};
