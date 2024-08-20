// Fill out your copyright notice in the Description page of Project Settings.


#include "ParentArea.h"

// Sets default values
AParentArea::AParentArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	sceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ParentComp"));
	RootComponent = sceneComponent;

	boxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	boxComponent->AttachToComponent(sceneComponent,FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void AParentArea::BeginPlay()
{
	Super::BeginPlay();
	boxComponent->SetVisibility(true);
	boxComponent->bHiddenInGame = false;
}

// Called every frame
void AParentArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

