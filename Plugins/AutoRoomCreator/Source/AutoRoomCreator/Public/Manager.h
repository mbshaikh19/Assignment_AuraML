// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Manager.generated.h"


// Struct to hold box size and other related properties
USTRUCT()
struct FBoxInfo
{
    GENERATED_BODY()

        FVector Size; // Size of the box
    FVector Position; // Position of the box

    FBoxInfo() : Size(FVector::ZeroVector), Position(FVector::ZeroVector) {}
    FBoxInfo(const FVector& InSize, const FVector& InPosition) : Size(InSize), Position(InPosition) {}
};

UCLASS()
class AUTOROOMCREATOR_API AManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Subarea")
    TArray<TSubclassOf<class AParentArea>> subareasToSpawn;

private:

    // Array to hold box components
    //TArray<UBoxComponent*> CollisionBoxes;
    TArray<FBoxInfo> BoxInfos; // Store sizes and positions

    // Seed for random number generation
    UPROPERTY(EditAnywhere, Category = "Randomization")
    int32 RandomSeed;

    // Number of boxes to place
    UPROPERTY(EditAnywhere, Category = "Settings")
    int32 NumberOfBoxes;

    // Room size (for example, width and length)
    UPROPERTY(EditAnywhere, Category = "Settings")
    FVector RoomDimensions;

    // Function to initialize and place collision boxes
    void SetupCollisionBoxes();

    // Helper function to check overlap
    bool IsPositionValid(const FVector& Position, const FVector& Size) const;

};
