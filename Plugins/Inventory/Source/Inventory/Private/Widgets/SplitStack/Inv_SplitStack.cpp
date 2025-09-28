// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SplitStack//Inv_SplitStack.h"
#include "Components/Slider.h"
#include "Components/SpinBox.h"
#include "Components/Button.h"
#include "Components/InventoryComponent.h"


void UInv_SplitStack::NativeConstruct()
{
	Super::NativeConstruct();

	if (PlayerInventory)
	{
		MaxStack = PlayerInventory->Inventory[SlotIndex].ItemNumericData.Quantity;
	}
	
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
	if (PlayerInventory)
	{
		if (SlotIndex >= 0)
		{
			PlayerInventory->SplitStack(SlotIndex, ValueToSplit);
			RemoveFromParent();
		}
	}
}

void UInv_SplitStack::OnSliderValueChanged(float Value)
{
	if (QuantitySpinBox->GetValue() != Value)
	{
		QuantitySpinBox->SetValue(Value);
		ValueToSplit = Value;
	}
	}

void UInv_SplitStack::OnSpinBoxValueChanged(float Value)
{
	if (QuantitySlider->GetValue() != Value)
	{
		QuantitySlider->SetValue(Value);
		ValueToSplit = Value;
	}
	}



