// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CPP_Pedestrian.generated.h"

UENUM(BlueprintType)
enum class ETraitValue : uint8
{
	LowScore UMETA(DisplayName = "LowValue"),
	MediumScore UMETA(DisplayName = "MediumValue"),
	HighScore UMETA(DisplayName = "HighValue")
};

UCLASS()
class CROWDSIMULATIONV4_API ACPP_Pedestrian : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACPP_Pedestrian();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Antropometric
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Antropometric", meta = (ExposeOnSpawn = "true"))
		ETraitValue Age; //Will influence speed and height
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Antropometric", meta = (ExposeOnSpawn = "true"))
		ETraitValue Height; //Will influence height
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Antropometric", meta = (ExposeOnSpawn = "true"))
		ETraitValue Weight; //Will influence size and speed
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Antropometric", meta = (ExposeOnSpawn = "true"))
		bool IsMale; //Will influence model

	//Pesonality
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = "Pesonality", meta = (ExposeOnSpawn = "true"))
		ETraitValue Openness; //Will influence Wander radius
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Pesonality", meta = (ExposeOnSpawn = "true"))
		ETraitValue Conscientiousness; //Will influence Wander blend
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Pesonality", meta = (ExposeOnSpawn = "true"))
		ETraitValue Extroversion; //Will influence RVO radius and cohesion or seperation
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Pesonality", meta = (ExposeOnSpawn = "true"))
		ETraitValue Agreeableness; //Will influence RVO blend and cohesion radius
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Pesonality", meta = (ExposeOnSpawn = "true"))
		ETraitValue Neuroticism; //Will influence cohesion blend

	//Variables
	int CohesionMultiplier = 1;
	UPROPERTY(BlueprintReadOnly)
		float WeightScale = 1.0f;
	float WanderRadius{500.0f};
	float SeekWanderBlend{ 1.0f };
	float CohesionRadius{ 500.0f };
	float CohesionBlend{1.0f};
	float Walkspeed{ 150.0f };
	FVector Scale = {1.0f,1.0f,1.0f};

	//Functions
	void GenerateTraits();
	void UpdatePersonalityTraits();
	void UpdateAntropometricTraits();

public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void DisableTraits();
	void EnableTraits();

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateVisualTraits();
	UFUNCTION(BlueprintImplementableEvent)
		void DefaultVisualTraits();

	//Getters
	ETraitValue GetOpenness() const { return Openness; }
	ETraitValue GetConscientiousness() const { return Conscientiousness; }
	ETraitValue GetNeuroticism() const { return Neuroticism; }
	ETraitValue GetAgreeableness() const { return Agreeableness; }
	ETraitValue GetExtroversion() const { return Extroversion; }

	int GetCohesionMultiplier() const { return CohesionMultiplier; }
	float GetWanderRadius() const { return WanderRadius; }
	float GetWanderSeekBlend() const { return SeekWanderBlend; }
	float GetCohesionBlend() const { return CohesionBlend; }
	float GetCohesionRadius() const { return CohesionRadius; }
	
};
