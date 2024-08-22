// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniAreaComponent.h"
#include "FloorAreaManager.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

UMiniAreaComponent::UMiniAreaComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	SetComponentTickEnabled(true);
}

void UMiniAreaComponent::BeginPlay()
{
	Super::BeginPlay();
	SetVisibility(true);
	if (AFloorAreaManager::GetFloorAreaManagerPtr())
	{
		////FScopeLock Lock(&delegateLock);
		//AFloorAreaManager::GetFloorAreaManagerPtr()->onSeedModify.AddDynamic(this, &UMiniAreaComponent::SetRandomSeed);
		////AFloorAreaManager::GetFloorAreaManagerPtr()->onSeedModify.AddLambda([this](int32 inSeed)
		////{
		////	SetRandomSeed(inSeed);
		////	UE_LOG(LogTemp, Warning, TEXT("oooo UMiniAreaComponent BeginPlay called"));
		////});
		//UE_LOG(LogTemp, Warning, TEXT("oooo UMiniAreaComponent BeginPlay binding DONE"));
	}
	//if (placeableObjectsList.Num() > 0)
	//{
	//	SpawnPlaceableActors();
	//}
	
	SetRandomSeed(50);
}

void UMiniAreaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FVector loc = GetAttachmentRoot()->GetComponentLocation() + GetRelativeLocation();
	UE_LOG(LogTemp, Warning, TEXT("OOOO tick called 2 %s"), *(loc.ToString()));
}

void UMiniAreaComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	ClearPreviousResult();
	UE_LOG(LogTemp, Warning, TEXT("oooo endplay in miniarea"));
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
	UE_LOG(LogTemp, Warning, TEXT("OOOO SpawnPlaceableActors called 1"));
	//FVector tempLocation = FVector::ZeroVector;
	if (placeableObjectsList.Num() == 1)
	{
		GetComponentLocation();
		//tempLocation = GetComponentLocation();
		
		FVector loc = GetComponentLocation();
		FRotator rot = GetComponentRotation();
		UE_LOG(LogTemp, Warning, TEXT("OOOO SpawnPlaceableActors spawn() called 2 %s"), *(loc.ToString()));
		APlaceableActor* spawnedTempPtr = GetWorld()->SpawnActor<APlaceableActor>(placeableObjectsList[0], loc, rot);
		spawnedPlaceableObjects.Add(spawnedTempPtr);
		return;
	}

	for (int32 index = 0; index < placeableObjectsList.Num(); index++)
	{
		UE_LOG(LogTemp, Warning, TEXT("OOOO SpawnPlaceableActors called 3"));
		APlaceableActor* spawnedTempPtr = GetWorld()->SpawnActor<APlaceableActor>(placeableObjectsList[index], FVector::ZeroVector, FRotator::ZeroRotator);
		spawnedPlaceableObjects.Add(spawnedTempPtr);
	}

	if (placeableObjectsList.Num() > 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("OOOO SpawnPlaceableActors called 4"));
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
	//if (spawnedPlaceableObjects.Num() > 0)
	//{
	UE_LOG(LogTemp, Warning, TEXT("OOOO SetRandomSeed called"));
		ClearPreviousResult();
		FTimerHandle timerHandle;
		GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &UMiniAreaComponent::SpawnPlaceableActors, 0.20f, false);
		//SpawnPlaceableActors();
	//}
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

