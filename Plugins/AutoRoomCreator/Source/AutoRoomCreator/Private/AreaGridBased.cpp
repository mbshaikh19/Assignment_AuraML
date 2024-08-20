// Fill out your copyright notice in the Description page of Project Settings.


#include "AreaGridBased.h"

void AAreaGridBased::BeginPlay()
{
	UActorComponent *actorComp = GetComponentByClass(UStaticMeshComponent::StaticClass());
	if (actorComp)
	{
		UStaticMeshComponent* staticComp = Cast<UStaticMeshComponent>(actorComp);
		if (staticComp)
		{
			FBox meshExtentBox = staticComp->Bounds.GetBox();
			FVector FloorPosition = meshExtentBox.GetCenter() - FVector(0.0f, 0.0f, meshExtentBox.GetSize().Z);
			if (objectsToSpawnList.Num() > 0)
			{
				GetWorld()->SpawnActor<AActor>(objectsToSpawnList[0], FloorPosition, FRotator::ZeroRotator);
			}
		}
	}
}

void AAreaGridBased::Tick(float DeltaTime)
{
}
