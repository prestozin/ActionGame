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

	if (SplitButton)
	{
		SplitButton->OnClicked.AddDynamic(this, &UInv_SplitStack::SplitConfirmed);
	}

	if (CancelButton)
	{
		CancelButton->OnClicked.AddDynamic(this, &UInv_SplitStack::CancelConfirmed);
	}

	if (DropButton)
	{
		DropButton->OnClicked.AddDynamic(this, &UInv_SplitStack::DropConfirmed);
	}
}

void UInv_SplitStack::SplitConfirmed()
{
	OnSplitConfirmed.ExecuteIfBound(SlotIndex, ValueToSplit);
	RemoveFromParent();
}

void UInv_SplitStack::CancelConfirmed()
{
	RemoveFromParent();
}

void UInv_SplitStack::DropConfirmed()
{
	int32 QuantityToDrop = QuantitySlider->GetValue();
	
	if (QuantityToDrop > 0)
	{
		OnDropConfirmed.ExecuteIfBound(SlotIndex, QuantityToDrop);
	}
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





