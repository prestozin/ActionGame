// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "UIManagerSubsystem.generated.h"

class UMainHud;
class UInventoryHUD;

UCLASS()
class ACTIONGAME_API UUIManagerSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()
protected:
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	
	UPROPERTY()
	UMainHud* MainHud;
	
public:
	
	UFUNCTION(BlueprintCallable)
	void CreateMainHUD();
	
	UFUNCTION(BlueprintCallable)
	void SetupInventory(UObject* InventorySource);

};
