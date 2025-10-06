// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inv_Filter.generated.h"

enum class EItemType : uint8;

#pragma region Delegates

DECLARE_DELEGATE_OneParam(FOnFilterClicked, EItemType&);

#pragma endregion
class UButton;

/**
 * 
 */
UCLASS()
class INVENTORY_API UInv_Filter : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	virtual void NativeConstruct() override;

	
	UFUNCTION()
	void OnFilterButtonClicked();
	
private:

	UPROPERTY(meta = (BindWidget))
	UButton* FilterButton;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	EItemType FilterType;
	
public:

	FOnFilterClicked OnFilterClicked;

};
