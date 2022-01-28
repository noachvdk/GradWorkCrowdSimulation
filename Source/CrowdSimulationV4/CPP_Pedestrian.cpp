// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Pedestrian.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
ACPP_Pedestrian::ACPP_Pedestrian()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ACPP_Pedestrian::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->bUseRVOAvoidance = true;

	//Generate traits
	GenerateTraits();
	//
	UpdatePersonalityTraits();
	//
	UpdateAntropometricTraits();
}

void ACPP_Pedestrian::GenerateTraits()
{
	//Generate random trait value physical
	Age = static_cast<ETraitValue>(FMath::RandRange(0, 2));
	Height = static_cast<ETraitValue>(FMath::RandRange(0, 2));
	Weight = static_cast<ETraitValue>(FMath::RandRange(0, 2));
	IsMale = FMath::RandBool();
	//Generate random trait value personality
	Openness = static_cast<ETraitValue>(FMath::RandRange(0, 2));
	Conscientiousness = static_cast<ETraitValue>(FMath::RandRange(0, 2));
	Extroversion = static_cast<ETraitValue>(FMath::RandRange(0, 2));
	Agreeableness = static_cast<ETraitValue>(FMath::RandRange(0, 2));
	Neuroticism = static_cast<ETraitValue>(FMath::RandRange(0, 2));
}

void ACPP_Pedestrian::UpdatePersonalityTraits()
{
	//Set values depending on traits
	//Agreeableness will decide how much importance will be on the RVO avoidance blend and how strong the cohesion is
	//Higher agreeableness will make it so that the character doesn't mind going closer to others
	if (Agreeableness == ETraitValue::LowScore)
	{
		GetCharacterMovement()->SetRVOAvoidanceWeight(0.75f);
		CohesionRadius = 300.f;
	}
	else if (Agreeableness == ETraitValue::MediumScore)
	{
		GetCharacterMovement()->SetRVOAvoidanceWeight(0.5f);
		CohesionRadius = 500.0f;
	}
	else
	{
		GetCharacterMovement()->SetRVOAvoidanceWeight(0.25f);
		CohesionRadius = 700.0f;
	}

	//Extroversion will decide how much importance will be on the RVO avoidance distance
	//Higher Extroversion will make it so that the character will only avoid others when close
	//if more introverted will go for seperation instead of cohesion
	if (Extroversion == ETraitValue::LowScore)
	{
		GetCharacterMovement()->AvoidanceConsiderationRadius = 700.0f;
		CohesionMultiplier = -1;
	}
	else if (Extroversion == ETraitValue::MediumScore)
	{
		GetCharacterMovement()->AvoidanceConsiderationRadius = 500.0;
		CohesionMultiplier = 0;
	}
	else
	{
		GetCharacterMovement()->AvoidanceConsiderationRadius = 300.0f;
		CohesionMultiplier = 1;
	}
	//Openness will decide how big the wander radius will be
	//Higher Openness will make it so that the character will wander further
	if (Openness == ETraitValue::LowScore)
		WanderRadius = 15.0f;
	else if (Openness == ETraitValue::MediumScore)
		WanderRadius = 30.0f;
	else
		WanderRadius = 45.0f;

	//Conscientiousness will decide how strong the wander blend is
	//Higher Conscientiousness will make it so that the character will stick more to it's seek
	if (Conscientiousness == ETraitValue::LowScore)
		SeekWanderBlend = 0.50f;
	else if (Conscientiousness == ETraitValue::MediumScore)
		SeekWanderBlend = 0.70f;
	else
		SeekWanderBlend = 0.90f;

	//Conscientiousness will decide how strong the wander blend is
	//Higher Conscientiousness will make it so that the character will stick more to it's seek
	if (Neuroticism == ETraitValue::LowScore)
		CohesionBlend = 0.25f;
	else if (Neuroticism == ETraitValue::MediumScore)
		CohesionBlend = 0.50f;
	else
		CohesionBlend = 0.75f;
}

void ACPP_Pedestrian::UpdateAntropometricTraits()
{
	//Age will have an influence on speed and height
	//Elderly will walk slowly
	Walkspeed = { 150.0f };
	float ScaleZ{ 0.75f }; //Elderly will be in between children height and adults
	if (Age == ETraitValue::LowScore)
	{
		Walkspeed = 250.0f;
		ScaleZ = 0.5f; //Children will be small
	}
	else if (Age == ETraitValue::MediumScore)
	{
		Walkspeed = 200.0f;
		ScaleZ = 1.0f; //Adults will be tall
	}

	//Height will depend on age but then will increase the base for that age depending on the trait
	const float difference{ 0.05f };
	const float HalfDifference{ difference / 2.0f };
	if (Height == ETraitValue::LowScore)
		Scale.Z = FMath::FRandRange(ScaleZ - (difference * 2.0f), ScaleZ - difference);
	else if (Height == ETraitValue::MediumScore)
		Scale.Z = FMath::FRandRange(ScaleZ - HalfDifference, ScaleZ + HalfDifference);
	else
		Scale.Z = FMath::FRandRange(ScaleZ + difference, ScaleZ + (difference * 2.0f));

	//Weight will affect the scale and walkSpeed
	//WeightScale will be used to scale the pelvis bone
	WeightScale = 1.0f;
	const float WeightSpeed{ 50.0f };
	if (Weight == ETraitValue::LowScore)
	{
		WeightScale = FMath::FRandRange(0.55f, 0.65f);
		Walkspeed += WeightSpeed;
	}
	else if (Weight == ETraitValue::MediumScore)
		WeightScale = FMath::FRandRange(0.95f, 1.05f);
	else
	{
		WeightScale = FMath::FRandRange(1.55f, 1.60f);
		Walkspeed -= WeightSpeed;
	}

	//Set parameters
	SetActorScale3D(Scale);
	GetCharacterMovement()->MaxWalkSpeed = Walkspeed;
	//Height, Weight and Gender will have visual results
	//The model changes will be done in the blueprint
	UpdateVisualTraits();
}

void ACPP_Pedestrian::EnableTraits()
{
	//Set parameters
	SetActorScale3D(Scale);
	GetCharacterMovement()->MaxWalkSpeed = Walkspeed;
	//Height, Weight and Gender will have visual results
	//The model changes will be done in the blueprint
	UpdateVisualTraits();
}

void ACPP_Pedestrian::DisableTraits()
{
	SetActorScale3D(FVector{ 1.0f,1.0f,1.0f });
	GetCharacterMovement()->MaxWalkSpeed = 150.0f;
	DefaultVisualTraits();
}

// Called to bind functionality to input
void ACPP_Pedestrian::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

