// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_ContextMenu.generated.h"

class UButton;

#pragma region Delegates

DECLARE_DELEGATE_OneParam(FOnEquipClicked, int32 /*slot index */)
DECLARE_DELEGATE_OneParam(FOnSplitClicked, int32 /*slot index */)
DECLARE_DELEGATE_OneParam(FOnDropClicked, int32 /*slot index */)

#pragma endregion
UCLASS()
class INVENTORY_API UInv_ContextMenu : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	virtual void NativeConstruct() override;
	
private:
	
	// ================================
	// =        PROPERTIES         =
	// ================================

	UPROPERTY()
	int32 SlotIndex = INDEX_NONE;
	
	UPROPERTY(meta = (BindWidget))
	UButton* EquipButton;
	
	UPROPERTY(meta = (BindWidget))
	UButton* SplitButton;

	UPROPERTY(meta = (BindWidget))
	UButton* DropButton;

	UPROPERTY(meta = (BindWidget))
	UButton* CancelButton;
	
	UPROPERTY(EditAnywhere, Category = "Menu Offset")
	FVector2D WidgetOffset;
	
	// ================================
	// =        FUNCTIONS         =
	// ================================
	
	UFUNCTION()
	void EquipItem();

	UFUNCTION()
	void OpenSplitWidget();

	UFUNCTION()
	void DropItem();



public:
	
	// ================================
	// =        DELEGATES         =
	// ================================

	FOnEquipClicked OnEquipClicked;
	FOnSplitClicked OnSplitClicked;
	FOnDropClicked	OnDropClicked;
	
	// ================================
	// =        FUNCTIONS         =
	// ================================

	UFUNCTION()
	void ToggleVisibility();

	UFUNCTION(BlueprintCallable)
	void SetContextSetup (int32 Index);

	FVector2D GetWidgetOffset() const;
};
