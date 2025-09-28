// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_SplitStack.generated.h"

class UInventoryComponent;
class USlider;
class USpinBox;
class UButton;

/**
 * 
 */


UCLASS()
class INVENTORY_API UInv_SplitStack : public UUserWidget
{
	GENERATED_BODY()

private:
	
	UPROPERTY(meta = (BindWidget))
	USpinBox* QuantitySpinBox;

	UPROPERTY(meta = (BindWidget))
	USlider* QuantitySlider;

	UPROPERTY(meta = (BindWidget))
	UButton* ConfirmSplit;

	UFUNCTION()
	void OnButtonClicked();
	
	UFUNCTION()
	void OnSliderValueChanged(float Value);

	UFUNCTION()
	void OnSpinBoxValueChanged(float Value);
	
public:
	
	UPROPERTY()
	UInventoryComponent* PlayerInventory;
	
	UPROPERTY()
	int32 MaxStack;
	
	UPROPERTY()
	int32 SlotIndex;
	
	UPROPERTY()
	int32 ValueToSplit;

protected:

	virtual void NativeConstruct() override; 

};
