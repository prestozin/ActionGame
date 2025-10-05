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

#pragma endregion


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

	void SetSplitSetup ();
	
public:
	
	UPROPERTY()
	int32 MaxStack;
	
	UPROPERTY()
	int32 SlotIndex;
	
	UPROPERTY()
	int32 ValueToSplit;

	FOnSplitConfirmed OnSplitConfirmed;

protected:

	virtual void NativeConstruct() override; 

};
