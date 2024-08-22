// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniAreaComponent.h"
#include "FloorAreaManager.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"

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
	//FVector loc = GetAttachmentRoot()->GetComponentLocation() + GetRelativeLocation();
	//UE_LOG(LogTemp, Warning, TEXT("OOOO tick called 2 %s"), *(loc.ToString()));
}

void UMiniAreaComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	ClearPreviousResult();
	UE_LOG(LogTemp, Warning, TEXT("oooo endplay in miniarea"));
}

void UMiniAreaComponent::GenerateSpawnLocations()
{
	FVector areaSize = GetScaledBoxExtent();
	FVector areaLocation = GetComponentLocation();
	//FTransform areaTransform = GetComponentTransform();
	FRotator areaRotation = GetComponentRotation();
	FQuat areaQuatRotation = FQuat(areaRotation);

	FVector areaSizeRotated = areaQuatRotation.RotateVector(areaSize);
	FVector areaLocationRotated = areaQuatRotation.RotateVector(areaLocation);

	FVector areaMin = FVector(areaLocation.X - areaSizeRotated.X, areaLocation.Y - areaSizeRotated.Y, 10.0f);
	FVector areaMax = FVector(areaLocation.X + areaSizeRotated.X, areaLocation.Y + areaSizeRotated.Y, 10.0f);

	UE_LOG(LogTemp, Warning, TEXT("oooo areaSize = %s rotation = %s"), *areaSize.ToString(), *areaRotation.ToString());

	FVector currentPosition = areaMin;
	FColor ranColor = FColor::MakeRandomColor();
	DrawDebugLine(GetWorld(), areaMin, FVector(areaMax.X, areaMin.Y, areaMin.Z), ranColor, false, 20, 0, 5);
	DrawDebugLine(GetWorld(), areaMin, FVector(areaMin.X, areaMax.Y, areaMin.Z), ranColor, false, 20, 0, 5);
	DrawDebugLine(GetWorld(), areaMax, FVector(areaMin.X, areaMax.Y, areaMin.Z), ranColor, false, 20, 0, 5);
	DrawDebugLine(GetWorld(), areaMax, FVector(areaMax.X, areaMin.Y, areaMin.Z), ranColor, false, 20, 0, 5);
	//DrawDebugBox(GetWorld(), areaLocationRotated, areaSize, FColor::MakeRandomColor(), false, 10.0f, 0, 3.0f);
	int32 placeableArrayLength = spawnedPlaceableObjects.Num();
	for (int32 index = 0; index < placeableArrayLength; index++)
	{
		//FVector actorSize = spawnedPlaceableObjects[index]->GetSize();
		//FVector currentActorSize = areaQuatRotation.RotateVector(actorSize);
		FVector currentActorSize = areaQuatRotation.RotateVector(spawnedPlaceableObjects[index]->GetSize());

		//if (currentPosition.X + currentActorSize.X <= areaMax.X && currentPosition.Y + currentActorSize.Y <= areaMax.Y)
		//{
		FVector newSpawnLocation = currentPosition +FVector(currentActorSize.X / 2.0f, currentActorSize.Y / 2.0f, 0.0f/*areaLocation.Z + (spawnedPlaceableObjects[index]->GetSize().Z / 2.0f) */ );
			spawnedPlaceableObjects[index]->SetActorLocation(newSpawnLocation);
		//	UE_LOG(LogTemp, Warning, TEXT("oooo in if Actor Name = %s  Spawned Actor name = %s  location = %s"), *GetAttachParentActor()->GetName(), *spawnedPlaceableObjects[index]->GetName(), *(newSpawnLocation.ToString()));
		//	currentPosition.X += currentActorSize.X;

		//	if (currentPosition.X + currentActorSize.X > areaMax.X)
		//	{
		//		currentPosition.Y += currentActorSize.Y;
		//		currentPosition.X = areaMin.X;

		//		if (currentPosition.Y + currentActorSize.Y > areaMax.Y)
		//		{
		//			return;
		//		}
		//	}
		//}

		//////////////////////////
		//FVector currentActorSizeRotated = areaSizeRotated;
		//bool bDoesBoxFit = currentPosition.X + currentActorSize.X <= areaMax.X && currentPosition.Y + currentActorSize.Y <= areaMax.Y;

		//bool bDoesRotatedBoxFit = currentPosition.X + currentActorSizeRotated.X <= areaMax.X &&	currentPosition.Y + currentActorSizeRotated.Y <= areaMax.Y;

		//if (bDoesBoxFit || bDoesRotatedBoxFit)
		//{
		//	FVector fitBoxSize;
		//	//FRotator BoxRotation = FRotator::ZeroRotator;

		//	if (bDoesBoxFit)
		//	{
		//		fitBoxSize = currentActorSize;
		//	}
		//	else if (bDoesRotatedBoxFit)
		//	{
		//		fitBoxSize = currentActorSizeRotated;
		//		//BoxRotation = FRotator(0, 90, 0);
		//	}

		//	FVector NewLocation(currentPosition.X + fitBoxSize.X / 2, currentPosition.Y + fitBoxSize.Y / 2, 0.0f);
		//	spawnedPlaceableObjects[index]->SetActorLocation(NewLocation);
		//	//spawnedPlaceableObjects[index]->SetActorRotation(BoxRotation);

		//	// Update the current position for the next box
		//	currentPosition.X += fitBoxSize.X;

		//	if (currentPosition.X + fitBoxSize.X > areaMax.X)
		//	{
		//		currentPosition.X = areaMin.X;
		//		currentPosition.Y += fitBoxSize.Y;
		//	}
		//}
		//else
		//{
		//	//spawnedPlaceableObjects[index]->Destroy();
		//	//spawnedPlaceableObjects.Remove(spawnedPlaceableObjects[index]);
		//	// Handle the case where the box doesn't fit
		//	UE_LOG(LogTemp, Warning, TEXT("Box %s does not fit in the defined area"), *spawnedPlaceableObjects[index]->GetName());
		//}
	}
}

void UMiniAreaComponent::SpawnPlaceableActors()
{
	//UE_LOG(LogTemp, Warning, TEXT("OOOO SpawnPlaceableActors called 1"));
	//FVector tempLocation = FVector::ZeroVector;
	if (placeableObjectsList.Num() == 1)
	{
		GetComponentLocation();
		//tempLocation = GetComponentLocation();
		
		
		FRotator rot = GetComponentRotation();
		//UE_LOG(LogTemp, Warning, TEXT("OOOO SpawnPlaceableActors spawn() called 2 %s"), *(loc.ToString()));
		APlaceableActor* spawnedTempPtr = GetWorld()->SpawnActor<APlaceableActor>(placeableObjectsList[0], FVector::ZeroVector, rot);
		FVector loc = GetComponentLocation() + FVector(0.0f, 0.0f, spawnedTempPtr->GetSize().Z/2.0f);
		spawnedTempPtr->SetActorLocation(loc);
		spawnedPlaceableObjects.Add(spawnedTempPtr);
		return;
	}

	for (int32 index = 0; index < placeableObjectsList.Num(); index++)
	{
		//UE_LOG(LogTemp, Warning, TEXT("OOOO SpawnPlaceableActors called 3"));
		APlaceableActor* spawnedTempPtr = GetWorld()->SpawnActor<APlaceableActor>(placeableObjectsList[index], FVector::ZeroVector, FRotator::ZeroRotator);
		spawnedPlaceableObjects.Add(spawnedTempPtr);
	}

	if (placeableObjectsList.Num() > 1)
	{
		//UE_LOG(LogTemp, Warning, TEXT("OOOO SpawnPlaceableActors called 4"));
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

