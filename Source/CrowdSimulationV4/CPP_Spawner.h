// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "CPP_Spawner.generated.h"

UCLASS()
class CROWDSIMULATIONV4_API ACPP_Spawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_Spawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Timer function
	void CheckForSpawn();

	UFUNCTION(BlueprintImplementableEvent)
	void SpawnPedestrian(const FVector& Location);

	//Variables
	UPROPERTY(BlueprintReadWrite)
	int CurrentSpawnedAmount{0};
	UPROPERTY(EditAnywhere, Category = "Spawning")
	int SpawnMaxAmount{20};
	UPROPERTY(EditAnywhere, Category = "Spawning")
	float SpawnTime{1.0f};


public:	
	UPROPERTY(Category = "CollisionMesh", EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* BoxCollision = nullptr;
};
