// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "PlaceableActor.h"
#include "MiniAreaComponent.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, meta = (BlueprintSpawnableComponent))
class AUTOROOMCREATOR_API UMiniAreaComponent : public UBoxComponent
{
	GENERATED_BODY()

private:
	TArray<FVector> spawnLocations;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Placeable")
	TArray<TSubclassOf<APlaceableActor>> placeableObjectsList;

	TArray<APlaceableActor*> spawnedPlaceableObjects;
	
protected:
	virtual void BeginPlay() override;

public:
	UMiniAreaComponent();

	void GenerateSpawnLocations();

	void SpawnPlaceableActors();

	void SortInDescendingOrder();
};
