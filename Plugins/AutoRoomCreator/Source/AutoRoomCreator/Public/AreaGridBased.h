// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ParentArea.h"
#include "Components/StaticMeshComponent.h"
#include "AreaGridBased.generated.h"

/**
 * 
 */
UCLASS()
class AUTOROOMCREATOR_API AAreaGridBased : public AParentArea
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	AAreaGridBased();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objects")
	TArray<TSubclassOf<AActor>> objectsToSpawnList;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Objects")
	UStaticMeshComponent* objetStaticMeshComp;
};
