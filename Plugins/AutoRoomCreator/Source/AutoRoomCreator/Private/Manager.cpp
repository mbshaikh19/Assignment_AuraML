// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager.h"
#include "Math/UnrealMathUtility.h"
#include "Engine/World.h"

// Sets default values
AManager::AManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set default values
	RandomSeed = 1234; // Default seed value
	NumberOfBoxes = 3;
	RoomDimensions = FVector(1000.f, 1000.f, 100.f);
}

// Called when the game starts or when spawned
void AManager::BeginPlay()
{
	Super::BeginPlay();
    SetupCollisionBoxes();
}

// Called every frame
void AManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AManager::SetupCollisionBoxes()
{
    // Initialize random stream with the seed
    FRandomStream RandomStream(RandomSeed);

    // Ensure we don�t have more boxes than the room can fit
    int32 Attempts = 0;
    const int32 MaxAttempts = 100;

    for (int32 i = 0; i < NumberOfBoxes; ++i)
    {
        FVector Size(
            RandomStream.FRandRange(30.f, 100.f), // Random size in X
            RandomStream.FRandRange(30.f, 100.f), // Random size in Y
            RandomStream.FRandRange(30.f, 100.f)  // Random size in Z
        );

        FVector Position;
        bool bPositionFound = false;

        while (Attempts < MaxAttempts)
        {
            // Generate random position within the room boundaries
            Position = FVector(
                RandomStream.FRandRange(-RoomDimensions.X / 2 + Size.X / 2, RoomDimensions.X / 2 - Size.X / 2),
                RandomStream.FRandRange(-RoomDimensions.Y / 2 + Size.Y / 2, RoomDimensions.Y / 2 - Size.Y / 2),
                Size.Z / 2 // Position the box on the floor
            );

            if (IsPositionValid(Position, Size))
            {
                bPositionFound = true;
                BoxInfos.Add(FBoxInfo(Size, Position));
                break;
            }

            ++Attempts;
        }

        if (bPositionFound)
        {
            UBoxComponent* BoxComponent = NewObject<UBoxComponent>(this);
            BoxComponent->RegisterComponent();
            BoxComponent->SetBoxExtent(Size / 2);
            BoxComponent->SetWorldLocation(Position);
            BoxComponent->SetCollisionProfileName(TEXT("BlockAll"));
            BoxComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
            CollisionBoxes.Add(BoxComponent);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to place all boxes without overlap."));
        }

        Attempts = 0; // Reset attempts for the next box
    }
}

bool AManager::IsPositionValid(const FVector& Position, const FVector& Size) const
{
    // Check overlap with existing boxes
    for (const FBoxInfo& BoxInfo : BoxInfos)
    {
        FVector ExistingBoxSize = BoxInfo.Size;
        FVector ExistingBoxPosition = BoxInfo.Position;

        // Compute the bounding boxes
        FBox NewBox(Position - Size / 2, Position + Size / 2);
        FBox ExistingBox(ExistingBoxPosition - ExistingBoxSize / 2, ExistingBoxPosition + ExistingBoxSize / 2);

        if (NewBox.Intersect(ExistingBox))
        {
            return false;
        }
    }
    return true;
}

