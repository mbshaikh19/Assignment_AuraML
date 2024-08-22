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
            //TriggerEvent(newSeed);
            
            TArray<AActor*> floorAreaManagers;
            UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFloorAreaManager::StaticClass(), floorAreaManagers);
            if (floorAreaManagers.Num() > 0)
            {
                if (Cast<AFloorAreaManager>(floorAreaManagers[0]))
                {
                    Cast<AFloorAreaManager>(floorAreaManagers[0])->SetRandomSeed(newSeed);
                    UE_LOG(LogTemp, Warning, TEXT("oooo Valid input seed. binding"));
                }
            }

                //if (AFloorAreaManager::GetFloorAreaManagerPtr())
                //{
                //    if (AFloorAreaManager::GetFloorAreaManagerPtr()->onSeedModify.IsBound())
                //    {
                //        FScopeLock Lock(&delegateLock);
                //        AFloorAreaManager::GetFloorAreaManagerPtr()->onSeedModify.Broadcast(newSeed);
                //        UE_LOG(LogTemp, Warning, TEXT("oooo Valid input seed. binding"));
                //    }
                //     else
                //     {
                //         UE_LOG(LogTemp, Warning, TEXT("oooo Valid input seed. no binding"));
                //     }
                //}


        }
        else {
            UE_LOG(LogTemp, Warning, TEXT("oooo Valid input seed. no binding"));
        }
    }
}

//void URoomGenratorWidget::TriggerEvent(int32 newSeed)
//{
//    FScopeLock Lock(&delegateLock);
//
//    if (AFloorAreaManager::GetFloorAreaManagerPtr())
//    {
//        if (AFloorAreaManager::GetFloorAreaManagerPtr()->onSeedModify.IsBound())
//        {
//            AFloorAreaManager::GetFloorAreaManagerPtr()->onSeedModify.Broadcast(newSeed);
//            UE_LOG(LogTemp, Warning, TEXT("oooo Valid input seed. binding"));
//        }
//         else
//         {
//             UE_LOG(LogTemp, Warning, TEXT("oooo Valid input seed. no binding"));
//         }
//    }
//}
