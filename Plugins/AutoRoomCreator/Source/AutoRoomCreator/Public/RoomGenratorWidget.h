 //Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "RoomGenratorWidget.generated.h"

/**
 * 
 */
UCLASS()
class AUTOROOMCREATOR_API URoomGenratorWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

protected:
    UPROPERTY(meta = (BindWidget))
    UButton* modifySeedButton;

    UPROPERTY(meta = (BindWidget))
    UEditableTextBox* seedInputTextBox;

    FCriticalSection delegateLock;

    UFUNCTION()
    void OnButtonClicked();

    //void TriggerEvent(int32 Value);
};
