// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "FloorAreaManager.generated.h"

class AParentArea;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSeedModify, int32, inSeed);

UCLASS()
class AUTOROOMCREATOR_API AFloorAreaManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloorAreaManager();

private:
	FVector2D areaMin;
	FVector2D areaMax;
	TArray<AParentArea*> spawnedSubareas;
	FRandomStream randomStream;

	static AFloorAreaManager* floorAreaManagerPtr;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SortInDescendingOrder();

	void FitIntoArea();

	void SpawnAndMakeList();

	void RandomizePlacement();

	UFUNCTION()
	void SetRandomSeed(int seedValue);

	FRandomStream GetRandomSeed() const;

	void ClearPreviousResult();

	static AFloorAreaManager* GetFloorAreaManagerPtr();

	//void TriggerEvent(int32 Value);
	//void AddBinding(UObject* object, TBaseDynamicDelegate<FNotThreadSafeDelegateMode,void,int32> delegate);
	//void ClearAllBindings();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ParentArea)
	UBoxComponent* boxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ParentArea)
	USceneComponent* sceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Subarea")
	TArray<TSubclassOf<class AParentArea>> subareasToSpawn;

	UPROPERTY()
	FOnSeedModify onSeedModify;

	FCriticalSection delegateLock;
};
