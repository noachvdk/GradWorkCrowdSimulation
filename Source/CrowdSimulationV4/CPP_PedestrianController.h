// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CPP_PathPoint.h"
#include "CPP_PedestrianController.generated.h"

class ACPP_CrowdManager;
class ACPP_Pedestrian;
class UNavigationSystemV1;
/**
 * 
 */
UCLASS()
class CROWDSIMULATIONV4_API ACPP_PedestrianController : public AAIController
{
	GENERATED_BODY()
protected:
	//Functions
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void CalcNextPoint();
	UFUNCTION(BlueprintCallable)
		void SetTargetAndDirection(FVector Target);
	UFUNCTION()
		void UpdateWanderTarget();
	//Toggling traits
	UFUNCTION(BlueprintCallable)
		void EnableTraits();
	UFUNCTION(BlueprintCallable)
		void DisableTraits();

	//Toggling traits
	UFUNCTION(BlueprintCallable)
		void ToggleDebug();

	void RetryCalcPoint();
	//Calc steering behaviors
	FVector CalculateSeek(const FVector& ActorLocation) const;
	FVector CalculateCohesionOrSeparation(const FVector& ActorLocation) const;
	FVector CalculateFinalTarget(const FVector& ActorLocation, const FVector& SeekTarget, const FVector& CohesionTarget, const float SeekBlend, const float CohesionBlend) const;


	//Variables
	//bools
	bool DrawDebug = false;
	UPROPERTY(BlueprintReadWrite)
		bool IsMoving = false;
	UPROPERTY(BlueprintReadWrite)
		bool UseTraits = true;
	//Radius for the sphereMulticast
	UPROPERTY(BlueprintReadOnly)
	float BaseCheckRadius = 100.0f;
	UPROPERTY(BlueprintReadWrite)
	float CurrentRadius = 100.0f;
	//Radius for get RandomReachablePoint
	UPROPERTY(BlueprintReadOnly)
		float BaseNavCheckRadius = 50.0f;
	UPROPERTY(BlueprintReadWrite)
		float CurrentNavCheckRadius = 50.0f;
	//Distances 
	const float FromActorDistance = 150.0f;
	const float AcceptanceRadius = 75.0f;
	UPROPERTY(BlueprintReadOnly)
	float DistanceToCheck = 500.0f;
	//Time check for when character stuck
	const float MaxStuckTime = 5.0f;
	float CurrentStuckTime = 0.0f;
	//Direction/Pos vectors 
	FVector TargetPosition{0.f,0.f,0.f};
	FVector TargetDirection{ 0.f,0.f,0.f };
	FVector WanderPos{ 0.f,0.f,0.f };
	//other objects
	UPROPERTY(BlueprintReadWrite)
	AActor* PathPoint = nullptr;
	UPROPERTY()
	ACPP_Pedestrian* ControlledPedestrian = nullptr;
	UPROPERTY(BlueprintReadWrite)
	ACPP_CrowdManager* CrowdManager = nullptr;
	UPROPERTY()
	UNavigationSystemV1* NavSys = nullptr;
public:
	ACPP_PedestrianController();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
