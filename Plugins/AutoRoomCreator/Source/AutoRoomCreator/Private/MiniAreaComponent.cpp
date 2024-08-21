// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniAreaComponent.h"
#include "FloorAreaManager.h"
#include "Kismet/GameplayStatics.h"

UMiniAreaComponent::UMiniAreaComponent()
{

}

void UMiniAreaComponent::BeginPlay()
{

	if (placeableObjectsList.Num() > 1)
	{
		SpawnPlaceableActors();
	}
	else if (placeableObjectsList.Num() == 1)
	{
		APlaceableActor* spawnedTempPtr = GetWorld()->SpawnActor<APlaceableActor>(placeableObjectsList[0], GetComponentLocation(), FRotator::ZeroRotator);
		spawnedPlaceableObjects.Add(spawnedTempPtr);
	}
}

void UMiniAreaComponent::GenerateSpawnLocations()
{
	FVector areaSize = (GetScaledBoxExtent() * 2.0f);
	FVector areaLocation = GetComponentLocation();

	FVector currentPosition = areaLocation - areaSize;

	for (int32 index = 0; index < spawnedPlaceableObjects.Num(); index++)
	{
		FVector currentActorSize = spawnedPlaceableObjects[index]->GetSize();

		if (currentPosition.X + currentActorSize.X > areaLocation.X + areaSize.X)
		{
			currentPosition.Y += currentActorSize.Y;
			currentPosition.X = areaLocation.X - areaSize.X;

			if (currentPosition.Y + currentActorSize.Y > areaLocation.Y + areaSize.Y)
			{
				return;
			}
		}

		FVector newSpawnLocation = currentPosition + FVector(currentActorSize.X / 2.0f, currentActorSize.Y / 2.0f, 0.0f);
		spawnedPlaceableObjects[index]->SetActorLocation(newSpawnLocation);

		currentPosition.X += currentActorSize.X;
	}
}

void UMiniAreaComponent::SpawnPlaceableActors()
{
	for (int32 index = 0; index < placeableObjectsList.Num(); index++)
	{
		APlaceableActor* spawnedTempPtr = GetWorld()->SpawnActor<APlaceableActor>(placeableObjectsList[index], FVector::ZeroVector, FRotator::ZeroRotator);
		spawnedPlaceableObjects.Add(spawnedTempPtr);
	}

	if (spawnedPlaceableObjects.Num() > 0)
	{
		SortInDescendingOrder();
		GenerateSpawnLocations();
	}
}

void UMiniAreaComponent::SortInDescendingOrder()
{
	spawnedPlaceableObjects.Sort([](const APlaceableActor& A, const APlaceableActor& B)->bool
		{
			if (!A.IsValidLowLevel() || !B.IsValidLowLevel())
				return false;

			int32 sizeA = A.GetSize().Length();
			int32 sizeB = B.GetSize().Length();

			return sizeA > sizeB;
		});
}
