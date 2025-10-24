// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainHud.generated.h"

class UCanvasPanel;
class UInventoryHUD;
/**
 * 
 */
UCLASS()
class ACTIONGAME_API UMainHud : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;

private:

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* CanvasHUD;
	
public:
	
	UPROPERTY(meta = (BindWidget))
	UInventoryHUD* InventoryHUD;
};
