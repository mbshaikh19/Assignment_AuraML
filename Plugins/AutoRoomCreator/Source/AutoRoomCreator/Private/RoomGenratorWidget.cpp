 //Fill out your copyright notice in the Description page of Project Settings.


#include "RoomGenratorWidget.h"
#include "FloorAreaManager.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/DefaultValueHelper.h"

void URoomGenratorWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (modifySeedButton)
    {
        modifySeedButton->OnClicked.AddDynamic(this, &URoomGenratorWidget::OnButtonClicked);
    }
}

void URoomGenratorWidget::OnButtonClicked()
{
    if (seedInputTextBox)
    {
        int32 newSeed = 0;
        if(FDefaultValueHelper::ParseInt(seedInputTextBox->GetText().ToString(), newSeed))
        {
            if (AFloorAreaManager::GetFloorAreaManagerPtr())
            {
                AFloorAreaManager::GetFloorAreaManagerPtr()->SetRandomSeed(newSeed);
                UE_LOG(LogTemp, Warning, TEXT("oooo Valid input seed."));
            }
            //TArray<AActor*> floorAreaManagers;
            //UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFloorAreaManager::StaticClass(), floorAreaManagers);
            //if (floorAreaManagers.Num() > 0)
            //{
            //    if (Cast<AFloorAreaManager>(floorAreaManagers[0]))
            //    {
            //        Cast<AFloorAreaManager>(floorAreaManagers[0])->SetRandomSeed(newSeed);
            //        
            //    }
            //}
        }
        else {
            UE_LOG(LogTemp, Warning, TEXT("oooo Invalid input seed."));
        }
    }
}