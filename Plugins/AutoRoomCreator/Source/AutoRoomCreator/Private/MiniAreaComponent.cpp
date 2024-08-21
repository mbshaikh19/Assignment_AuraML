// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniAreaComponent.h"
#include "FloorAreaManager.h"
#include "Kismet/GameplayStatics.h"

UMiniAreaComponent::UMiniAreaComponent()
{

}

void UMiniAreaComponent::BeginPlay()
{
	Super::BeginPlay();
	if (AFloorAreaManager::GetFloorAreaManagerPtr())
	{
		FScopeLock Lock(&delegateLock);
		AFloorAreaManager::GetFloorAreaManagerPtr()->onSeedModify.AddDynamic(this, &UMiniAreaComponent::SetRandomSeed);
		UE_LOG(LogTemp, Warning, TEXT("oooo UMiniAreaComponent BeginPlay binding DONE"));
	}
	//if (placeableObjectsList.Num() > 0)
	//{
	//	SpawnPlaceableActors();
	//}
}

void UMiniAreaComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
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
	FVector tempLocation = FVector::ZeroVector;
	if (spawnedPlaceableObjects.Num() == 1)
	{
		tempLocation = GetComponentLocation();
	}

	for (int32 index = 0; index < placeableObjectsList.Num(); index++)
	{
		APlaceableActor* spawnedTempPtr = GetWorld()->SpawnActor<APlaceableActor>(placeableObjectsList[index], tempLocation, FRotator::ZeroRotator);
		spawnedPlaceableObjects.Add(spawnedTempPtr);
	}

	if (spawnedPlaceableObjects.Num() > 1)
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

void UMiniAreaComponent::SetRandomSeed(int seedValue)
{
	if (spawnedPlaceableObjects.Num() > 0)
	{
		ClearPreviousResult();
		SpawnPlaceableActors();
	}
}

void UMiniAreaComponent::ClearPreviousResult()
{

	for (int32 index = 0; index < spawnedPlaceableObjects.Num(); index++)
	{
		if (spawnedPlaceableObjects[index])
		{
			spawnedPlaceableObjects[index]->Destroy();
		}
	}
	spawnedPlaceableObjects.Empty();

}

