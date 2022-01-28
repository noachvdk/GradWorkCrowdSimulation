// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPP_CrowdManager.generated.h"

class ACPP_Pedestrian;
UCLASS()
class CROWDSIMULATIONV4_API ACPP_CrowdManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_CrowdManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	TArray<ACPP_Pedestrian*> Pedestrians{};
public:	
	UFUNCTION(BlueprintCallable)
	void AddPedestrian(ACPP_Pedestrian* Pedestrian);

	TArray<ACPP_Pedestrian*> GetAllPedestriansInRadiusFromPoint(const ACPP_Pedestrian* agent, float radius);
	FVector GetCenterOfPedestriansInRadius(const ACPP_Pedestrian* agent, float radius);

};
