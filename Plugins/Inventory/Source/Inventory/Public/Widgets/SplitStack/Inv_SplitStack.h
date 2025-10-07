// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_SplitStack.generated.h"

#pragma region Classes

class USlider;
class USpinBox;
class UButton;

#pragma endregion

#pragma region Delegates

DECLARE_DELEGATE_TwoParams(FOnSplitConfirmed, int32 /*SlotIndex*/, int32 /*Quantity*/);
DECLARE_DELEGATE_TwoParams(FOnDropConfirmed, int32 /*SlotIndex*/, int32 /*DropQuantity*/);

#pragma endregion


UCLASS()
class INVENTORY_API UInv_SplitStack : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	virtual void NativeConstruct() override;
	
private:

	// ================================
	// =        PROPERTIES            =
	// ================================
	
	UPROPERTY(meta = (BindWidget))
	USpinBox* QuantitySpinBox;

	UPROPERTY(meta = (BindWidget))
	USlider* QuantitySlider;

	UPROPERTY(meta = (BindWidget))
	UButton* SplitButton;

	UPROPERTY(meta = (BindWidget))
	UButton* CancelButton;

	UPROPERTY(meta = (BindWidget))
	UButton* DropButton;

	// ================================
	// =        FUNCTIONS           =
	// ================================
	
	UFUNCTION()
	void SplitConfirmed();

	UFUNCTION()
	void CancelConfirmed();
	
	UFUNCTION()
	void DropConfirmed();
	
	UFUNCTION()
	void OnSliderValueChanged(float Value);

	UFUNCTION()
	void OnSpinBoxValueChanged(float Value);

	void SetSplitSetup ();
	
public:
	
	// ================================
	// =        PROPERTIES            =
	// ================================
	
	UPROPERTY()
	int32 MaxStack;
	
	UPROPERTY()
	int32 SlotIndex;
	
	UPROPERTY()
	int32 ValueToSplit;

	// ================================
	// =        DELEGATES          =
	// ================================

	FOnSplitConfirmed OnSplitConfirmed;
	
	FOnDropConfirmed OnDropConfirmed;
};
