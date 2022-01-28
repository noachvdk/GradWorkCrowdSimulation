// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_CrowdManager.h"
#include "CPP_Pedestrian.h"

// Sets default values
ACPP_CrowdManager::ACPP_CrowdManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ACPP_CrowdManager::BeginPlay()
{
	Super::BeginPlay();
}

void ACPP_CrowdManager::AddPedestrian(ACPP_Pedestrian* Pedestrian)
{
	if(IsValid(Pedestrian))
		Pedestrians.Push(Pedestrian);
}

TArray<ACPP_Pedestrian*> ACPP_CrowdManager::GetAllPedestriansInRadiusFromPoint(const ACPP_Pedestrian* agent, float radius)
{
	TArray<ACPP_Pedestrian*> PedestriansInRadius{};

	if(Pedestrians.Num() == 0)
		return PedestriansInRadius;

	//loop over all possible pedestrians
	for(ACPP_Pedestrian* Pedestrian : Pedestrians)
	{
		//check if valid and not the same agent
		if(IsValid(Pedestrian) && agent != Pedestrian)
		{
			//get distance
			FVector distance = Pedestrian->GetActorLocation() - agent->GetActorLocation();
			//If within the radius add it to the array
			if(distance.Size() <= radius)
			{
				PedestriansInRadius.Push(Pedestrian);
			}
		}
	}
	return PedestriansInRadius;
}

FVector ACPP_CrowdManager::GetCenterOfPedestriansInRadius(const ACPP_Pedestrian* agent, float radius)
{
	//Get all pedestrians in the radius
	TArray<ACPP_Pedestrian*> PedestriansInRadius{GetAllPedestriansInRadiusFromPoint(agent,radius)};
	//if no return empty vector
	FVector CenterPoint{0.f,0.f,0.f};
	if (PedestriansInRadius.Num() == 0)
		return CenterPoint;

	//else add all locations to point 
	for (const ACPP_Pedestrian* Pedestrian : PedestriansInRadius)
	{
		CenterPoint += Pedestrian->GetActorLocation();
	}
	//then divide by the amount to get the center
	CenterPoint /= PedestriansInRadius.Num();
	return CenterPoint;
}

