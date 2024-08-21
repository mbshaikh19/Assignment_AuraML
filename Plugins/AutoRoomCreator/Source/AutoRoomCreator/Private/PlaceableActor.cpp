// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaceableActor.h"

// Sets default values
APlaceableActor::APlaceableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	objectStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Object_SM"));
	RootComponent = objectStaticMesh;
}

// Called when the game starts or when spawned
void APlaceableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlaceableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector APlaceableActor::GetSize() const
{
	if (objectStaticMesh)
	{
		return (objectStaticMesh->Bounds.BoxExtent) * 2.0f;
	}
	return FVector::ZeroVector;
}

