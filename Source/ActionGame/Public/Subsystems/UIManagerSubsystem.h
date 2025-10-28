// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "UIManagerSubsystem.generated.h"

class UMyGameInstance;
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
	
	// ================================
	// =        PROPERTIES           =
	// ================================
	
	UPROPERTY()
	TObjectPtr<UWorld> World;

	UPROPERTY()
	TObjectPtr<APlayerController> OwningPlayer;
	
	UPROPERTY()
	TObjectPtr<UMainHud> MainHud;

	UPROPERTY()
	TObjectPtr<UInventoryHUD> InventoryHUD;

	UPROPERTY()
	TObjectPtr<UMyGameInstance> MyGameInstance;
	
	// ================================
	// =        TEMPLATES           =
	// ================================
	
	template <typename WidgetType>
	TObjectPtr<WidgetType> CreateWidgetOfClass(TSubclassOf<WidgetType> WidgetClass)
	{
		if (!World || !WidgetClass) return nullptr;
		
		TObjectPtr<WidgetType> Widget = CreateWidget<WidgetType>(OwningPlayer, WidgetClass);
		if (!Widget) return nullptr;
		return Widget;
	}
	
	// ================================
	// =        FUNCTIONS            =
	// ================================
	
	void SetDefaults();
	
public:
	
	// ================================
	// =        FUNCTIONS            =
	// ================================
	
	void CreateHUD();
	
	void CreateInventoryWidget();
	
	void SetInventoryWidget(TObjectPtr<UObject> InventorySource);

	void ToggleInventoryWidget() const;

	void OnPlayerControllerReady();

};
