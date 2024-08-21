// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "PlaceableActor.generated.h"

UCLASS()
class AUTOROOMCREATOR_API APlaceableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* objectStaticMesh;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this actor's properties
	APlaceableActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FVector GetSize() const;

};
