// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_PedestrianController.h"
#include "CPP_Pedestrian.h"
#include "CPP_CrowdManager.h"
#include "DrawDebugHelpers.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "Kismet/GameplayStatics.h"

ACPP_PedestrianController::ACPP_PedestrianController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACPP_PedestrianController::BeginPlay()
{
	Super::BeginPlay();
	NavSys = UNavigationSystemV1::GetCurrent(GetWorld());

	//Calc wander pos at intervals to avoid jittering
	FTimerHandle WanderHandle{};
	GetWorld()->GetTimerManager().SetTimer(WanderHandle, this, &ACPP_PedestrianController::UpdateWanderTarget, 0.5f, true, 0.1f);
}

void ACPP_PedestrianController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//If pawn not valid retry getting pawn
	if (!IsValid(ControlledPedestrian))
	{
		ControlledPedestrian = Cast<ACPP_Pedestrian>(GetPawn());
		//rGEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red, FString::Printf(TEXT("retry getting pedestrian")));
		if (IsValid(ControlledPedestrian))
		{
			//GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Red, FString::Printf(TEXT("retry succesfull")));
			CalcNextPoint();
		}
		return;
	}

	//General variables
	const FVector ActorLocation{ ControlledPedestrian->GetActorLocation() };
	FVector FinalTarget{ 0.0f,0.f,0.f };

	//Only when it has found a point and is using traits calculate the final target with the different steering behaviors
	if(UseTraits && IsMoving)
	{
		//Set seek target
		const FVector SeekTarget{ CalculateSeek(ActorLocation) };
		if (DrawDebug)DrawDebugLine(GetWorld(), ActorLocation, SeekTarget, FColor::Green, false, 0.1f);
		
		//Wander update done with timer function to avoid jittering
		if (DrawDebug)DrawDebugLine(GetWorld(), ActorLocation, WanderPos, FColor::Blue, false, 0.1f);

		//Set Cohesion or Separation target depending on traits
		const FVector CohesionOrSeparationTarget{ CalculateCohesionOrSeparation(ActorLocation) };
		if (DrawDebug)DrawDebugLine(GetWorld(), ActorLocation, CohesionOrSeparationTarget, FColor::Red, false, 0.1f);

		//SeekBlend will be influenced by Conscientiousness
		//Lower Conscientiousness will mean that the character is more impulsive so he will lean more towards the wander.
		const float SeekBlend{ ControlledPedestrian->GetWanderSeekBlend() };
		//Cohesion will be influenced by Neuroticism; 
		//Higher Neuroticism will mean that the character will place more importance on the cohesion
		const float CohesionBlend{ ControlledPedestrian->GetCohesionBlend() };

		//Final blend
		FinalTarget = { CalculateFinalTarget(ActorLocation, SeekTarget,CohesionOrSeparationTarget,SeekBlend,CohesionBlend) };
		if (DrawDebug)DrawDebugLine(GetWorld(), ActorLocation, FinalTarget, FColor::Black, false, 0.1f);
	}
	else if(IsMoving)
	{
		//If not using the traits will go straight towards the target using seek
		FinalTarget = CalculateSeek(ActorLocation);
	}

	//Execute the actual movement
	if (IsMoving)
	{
		//make sure that the target point is a reachable point
		FNavLocation Result{};
		bool Succeeded{false};
		//Using the for loop in the case that if it doesn't find a point it would get stuck spinning
		for (int i{ 1 }; i <= 10; i++)
		{
			Succeeded = NavSys->GetRandomPointInNavigableRadius(FinalTarget, (i * 10.0f), Result);
			if (Succeeded)
				break;
		}

		//Move
		if(Succeeded)
		{
			//Move towards target point
			const EPathFollowingRequestResult::Type result = MoveToLocation(Result.Location, AcceptanceRadius); // this way it will keep going towards the target direction
			if (result == EPathFollowingRequestResult::Failed || result == EPathFollowingRequestResult::AlreadyAtGoal)
			{
				//UE_LOG(LogTemp, Warning, TEXT("failed to move to point"));
				RetryCalcPoint();
				//GEngine->AddOnScreenDebugMessage(0, 2.f, FColor::Red, FString::Printf(TEXT("failed to move to point")));
			}
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("failed to find point"));
			ControlledPedestrian->SetActorLocation(ActorLocation + (ControlledPedestrian->GetActorForwardVector() * 10.0f));
			RetryCalcPoint();
		}

		//check if point reached
		const float Distance = FVector::Distance(ActorLocation, TargetPosition);
		if (Distance <= AcceptanceRadius)
		{
			//if in radius of target point stop wandering and calc next point
			IsMoving = false;
			//UE_LOG(LogTemp, Warning, TEXT("point reached"));
			CalcNextPoint();
		}
	}

	//if pawn valid but not moving while not looking for a point retry the path logic
	if (IsValid(ControlledPedestrian) && ControlledPedestrian->GetVelocity().Size() <= 15.0f && IsMoving) 
	{
		CurrentStuckTime += DeltaTime;
		if(CurrentStuckTime >= MaxStuckTime) //the pedestrian has been stuck for some time reset the movement logic
		{
			CurrentStuckTime = 0.0f; //reset timer
			//UE_LOG(LogTemp, Warning, TEXT("stuck"));
			RetryCalcPoint();
			//GEngine->AddOnScreenDebugMessage(0, 5.f, FColor::Red, FString::Printf(TEXT("stuck so is resetting")));
		}
	}
	else
		CurrentStuckTime = 0.0f;
}

void ACPP_PedestrianController::SetTargetAndDirection(FVector Target)
{
	if(ControlledPedestrian)
	{
		TargetPosition = Target;
		TargetDirection = TargetPosition - ControlledPedestrian->GetActorLocation();
		TargetDirection = TargetDirection.GetSafeNormal();
		IsMoving = true;
	}
}

FVector ACPP_PedestrianController::CalculateSeek(const FVector& ActorLocation) const
{
	//Calculate the direction
	FVector TempDirection{ TargetPosition - ActorLocation };
	TempDirection = TempDirection.GetSafeNormal();
	return (ActorLocation + (TempDirection * FromActorDistance));//multiply by custom value
}

void ACPP_PedestrianController::UpdateWanderTarget() 
{
	//only update if moving and valid
	if (!IsValid(ControlledPedestrian) || !IsMoving)
		return;

	//Jitter will be influenced by Openness
	//Lower Openness will mean that the character prefers routine so he will follow his usual path more so less jitter.
	const float MaxJitter{ControlledPedestrian->GetWanderRadius()};
	const float yOffset{ FMath::FRandRange(-MaxJitter, MaxJitter) };

	//Rotate forward vector by with the offset and multiply with radius to get wander pos
	const FRotator WanderRotation(0, yOffset, 0);
	const FVector WanderDir = WanderRotation.RotateVector(TargetDirection);
	WanderPos = ControlledPedestrian->GetActorLocation() + (WanderDir * FromActorDistance);
}

FVector ACPP_PedestrianController::CalculateCohesionOrSeparation(const FVector& ActorLocation) const 
{
	//Cohesion or Separation depending on traits
	if (IsValid(CrowdManager))
	{
		const float CohesionRadius{ ControlledPedestrian->GetCohesionRadius() }; //Size of radius depends on traits
		//Get all the other characters that are close and calc the middle point
		const FVector NeighbourMiddlePoint{ CrowdManager->GetCenterOfPedestriansInRadius(ControlledPedestrian,CohesionRadius) };
		const int CohesionOrSeparationMulitplier{ ControlledPedestrian->GetCohesionMultiplier()};

		if (NeighbourMiddlePoint != FVector{ 0.f,0.f,0.f }) //only if other pedestrians are found
		{
			FVector CohesionDirection = { NeighbourMiddlePoint - ActorLocation };//calc direction between character and middle point
			CohesionDirection *= CohesionOrSeparationMulitplier; //switch going towards or away from center depending on trait
			CohesionDirection = CohesionDirection.GetSafeNormal();
			return (ActorLocation + (CohesionDirection * FromActorDistance));//multiply by custom value
		}
	}
	//No cohesion or separation will be used
	return FVector{ 0.f,0.f,0.f };
}

FVector ACPP_PedestrianController::CalculateFinalTarget(const FVector& ActorLocation, const FVector& SeekTarget, const FVector& CohesionTarget, const float SeekBlend, const float CohesionBlend) const
{
	FVector FinalDirection{ 0.f,0.f,0.f };
	//If there were no close pedestrians there will be no need for cohesion or seperation
	if (CohesionTarget != FVector{ 0.f,0.f,0.f })
	{
		//Recalc the direction but with the blend values
		FVector WanderDirection = WanderPos - ActorLocation;
		WanderDirection *= (1.0f - CohesionBlend);
		FVector CohesionDirection = CohesionTarget - ActorLocation;
		CohesionDirection *= CohesionBlend;

		//Then blend between the cohesion and wander
		FVector CohesionWanderBlendDirection{ (CohesionDirection)+(WanderDirection) };
		CohesionWanderBlendDirection = CohesionWanderBlendDirection.GetSafeNormal();
		CohesionWanderBlendDirection *= FromActorDistance;
		if (DrawDebug)DrawDebugLine(GetWorld(), ActorLocation, ActorLocation + CohesionWanderBlendDirection, FColor::Orange, false, 0.1f);

		//then between the seek and blended behavior the min seek strength is 0.5f
		FVector SeekDirection = SeekTarget - ActorLocation;
		SeekDirection *= SeekBlend;
		//Apply the blend to previously calculated blended direction
		CohesionWanderBlendDirection *= (1.0f - SeekBlend);

		//Recalc the direction but with the blend values
		FinalDirection = (SeekDirection)+(CohesionWanderBlendDirection);
	}
	else
	{
		//Recalc the direction but with the blend values
		FVector SeekDirection = SeekTarget - ActorLocation;
		SeekDirection *= SeekBlend;
		FVector WanderDirection = WanderPos - ActorLocation;
		WanderDirection *= (1.0f - SeekBlend);
		//Base behavior blend between the wander and seek the min seek strength is 0.5f
		FinalDirection = (SeekDirection)+(WanderDirection);
	}

	//normalize and multiply with custom value
	FinalDirection = FinalDirection.GetSafeNormal();
	FinalDirection *= FromActorDistance;
	return FinalDirection + ActorLocation;
}

void ACPP_PedestrianController::RetryCalcPoint()
{
	IsMoving = false;
	const FRotator Rotation{ 0.0f,15.0f,0.f };
	ControlledPedestrian->AddActorLocalRotation(Rotation); //rotate
	CalcNextPoint(); // if point doesn't work try a new point
}

void ACPP_PedestrianController::EnableTraits()
{
	UseTraits = true;

	if(IsValid(ControlledPedestrian))
	{
		ControlledPedestrian->EnableTraits();
	}
}

void ACPP_PedestrianController::DisableTraits()
{
	UseTraits = false;
	if (IsValid(ControlledPedestrian))
	{
		ControlledPedestrian->DisableTraits();
	}
}

void ACPP_PedestrianController::ToggleDebug()
{
	DrawDebug = !DrawDebug;
}
