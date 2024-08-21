// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorAreaManager.h"
#include "Math/UnrealMathUtility.h"
#include "ParentArea.h"

AFloorAreaManager* AFloorAreaManager::floorAreaManagerPtr = nullptr;

// Sets default values
AFloorAreaManager::AFloorAreaManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ParentComp"));
	RootComponent = sceneComponent;

	boxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	boxComponent->AttachToComponent(sceneComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void AFloorAreaManager::BeginPlay()
{
	Super::BeginPlay();

    //FOnSeedModify::FDelegate delegate;
    //delegate.BindUFunction(this, TEXT("SetRandomSeed"));
    //AddBinding(this, delegate);
    FScopeLock Lock(&delegateLock);
    onSeedModify.AddDynamic(this, &AFloorAreaManager::SetRandomSeed);
    if (floorAreaManagerPtr == nullptr)
    {
        floorAreaManagerPtr = this;
    }

	if (boxComponent)
	{
		// Get the box extent (half-size) and the component's location
		FVector BoxExtent = boxComponent->GetScaledBoxExtent();
		FVector BoxCenter = boxComponent->GetComponentLocation();

		// Calculate min and max points
		areaMin = FVector2D(BoxCenter.X - BoxExtent.X, BoxCenter.Y - BoxExtent.Y);
		areaMax = FVector2D(BoxCenter.X + BoxExtent.X, BoxCenter.X + BoxExtent.X);
	}
    //SetRandomSeed(20);
}

void AFloorAreaManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    onSeedModify.Clear();
    UE_LOG(LogTemp, Warning, TEXT("oooo delegate cleared"));
}

// Called every frame
void AFloorAreaManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFloorAreaManager::SortInDescendingOrder()
{
    spawnedSubareas.Sort([](const AParentArea& A, const AParentArea& B)->bool
        {
            if (!A.IsValidLowLevel() || !B.IsValidLowLevel())
                return false;

            return A.GetSize() > B.GetSize();
        });
}

void AFloorAreaManager::FitIntoArea()
{
    FVector2D CurrentPosition = areaMin;

    for (int32 index = 0; index < spawnedSubareas.Num(); index++)
    {
        AParentArea* subarea = spawnedSubareas[index];
        if (!subarea) 
            continue;

        FVector2D BoxSize(subarea->GetAreaSize()); 
        FVector2D RotatedBoxSize(subarea->GetAreaSize().Y, subarea->GetAreaSize().X);  // Size if rotated by 90 degrees

        bool bDoesBoxFit = CurrentPosition.X + BoxSize.X <= areaMax.X &&
            CurrentPosition.Y + BoxSize.Y <= areaMax.Y;

        bool bDoesRotatedBoxFit = CurrentPosition.X + RotatedBoxSize.X <= areaMax.X &&
            CurrentPosition.Y + RotatedBoxSize.Y <= areaMax.Y;

        if (bDoesBoxFit || bDoesRotatedBoxFit)
        {
            FVector2D fitBoxSize;
            FRotator BoxRotation = FRotator::ZeroRotator;

            if (bDoesBoxFit)
            {
                fitBoxSize = BoxSize;
            }
            else if (bDoesRotatedBoxFit)
            {
                fitBoxSize = RotatedBoxSize;
                BoxRotation = FRotator(0, 90, 0);
            }

            FVector NewLocation(CurrentPosition.X + fitBoxSize.X / 2, CurrentPosition.Y + fitBoxSize.Y / 2, 0.0f);
            subarea->SetActorLocation(NewLocation);
            subarea->SetActorRotation(BoxRotation);

            // Update the current position for the next box
            CurrentPosition.X += fitBoxSize.X;

            if (CurrentPosition.X + fitBoxSize.X > areaMax.X)
            {
                CurrentPosition.X = areaMin.X;
                CurrentPosition.Y += fitBoxSize.Y;
            }
        }
        else
        {
            subarea->Destroy();
            spawnedSubareas.Remove(subarea);
            // Handle the case where the box doesn't fit
            UE_LOG(LogTemp, Warning, TEXT("Box %s does not fit in the defined area"), *subarea->GetName());
        }
    }
}

void AFloorAreaManager::SpawnAndMakeList()
{
    for (TSubclassOf<AParentArea> subareaSpawnClass : subareasToSpawn)
    {
        AParentArea* parentArea = GetWorld()->SpawnActor<AParentArea>(subareaSpawnClass, FVector::ZeroVector, FRotator::ZeroRotator);
        spawnedSubareas.Add(parentArea);
    }

    if (spawnedSubareas.Num() > 0)
    {
        //SortInDescendingOrder();
        FitIntoArea();
    }
}

void AFloorAreaManager::RandomizePlacement()
{
    for (int index = subareasToSpawn.Num() - 1; index > 0; --index)
    {
        int randomElement = randomStream.RandHelper(index);
        subareasToSpawn.Swap(index, randomElement);
    }
    SpawnAndMakeList();
}

void AFloorAreaManager::SetRandomSeed(int seedValue)
{
    UE_LOG(LogTemp, Warning, TEXT("oooo Valid input seed received in floor area manager."));
    ClearPreviousResult();
    randomStream.Initialize(seedValue);
    RandomizePlacement();
}


void AFloorAreaManager::ClearPreviousResult()
{
    if (spawnedSubareas.Num() > 0)
    {
        for (int32 index = 0; index < spawnedSubareas.Num(); index++)
        {
            if (spawnedSubareas[index]->IsValidLowLevel())
            {
                spawnedSubareas[index]->Destroy();
            }
        }
        spawnedSubareas.Empty();
    }
}


FRandomStream AFloorAreaManager::GetRandomSeed() const
{
    return randomStream;
}

AFloorAreaManager* AFloorAreaManager::GetFloorAreaManagerPtr()
{
    return floorAreaManagerPtr;
}

//void AFloorAreaManager::AddBinding(UObject* object, TBaseDynamicDelegate<FNotThreadSafeDelegateMode, void, int32> delegate)
//{
//    FScopeLock Lock(&delegateLock);
//    onSeedModify.AddDynamic(this, delegate);
//    UE_LOG(LogTemp, Warning, TEXT("oooo AddBinding called."));
//}
//
//void AFloorAreaManager::ClearAllBindings()
//{
//    FScopeLock Lock(&delegateLock);
//    onSeedModify.Clear();
//    UE_LOG(LogTemp, Warning, TEXT("oooo ClearAllBindings called."));
//}


