// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SplitStack//Inv_SplitStack.h"
#include "Components/Slider.h"
#include "Components/SpinBox.h"
#include "Components/Button.h"



void UInv_SplitStack::NativeConstruct()
{
	Super::NativeConstruct();
	
	SetSplitSetup();
}

void UInv_SplitStack::SetSplitSetup()
{
	if (QuantitySlider)
	{
		QuantitySlider->OnValueChanged.AddDynamic(this, &UInv_SplitStack::OnSliderValueChanged);
		QuantitySlider->SetMaxValue(MaxStack);
	}

	if (QuantitySpinBox)
	{
		QuantitySpinBox->OnValueChanged.AddDynamic(this, &UInv_SplitStack::OnSpinBoxValueChanged);
		QuantitySpinBox->SetMaxValue(MaxStack);
	}

	if (ConfirmSplit)
	{
		ConfirmSplit->OnClicked.AddDynamic(this, &UInv_SplitStack::OnButtonClicked);
	}
}

void UInv_SplitStack::OnButtonClicked()
{
	OnSplitConfirmed.ExecuteIfBound(SlotIndex, ValueToSplit);
	RemoveFromParent();
}

void UInv_SplitStack::OnSliderValueChanged(float Value)
{
	if (!(QuantitySpinBox->GetValue() != Value)) return;
	
	QuantitySpinBox->SetValue(Value);
	ValueToSplit = Value;
}

void UInv_SplitStack::OnSpinBoxValueChanged(float Value)
{
	if (!(QuantitySlider->GetValue() != Value)) return;
	
	QuantitySlider->SetValue(Value);
	ValueToSplit = Value;
}





