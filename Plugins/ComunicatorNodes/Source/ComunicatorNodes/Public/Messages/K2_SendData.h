// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "K2Node.h"
#include "K2Node_AddPinInterface.h"
#include "K2_SendData.generated.h"

USTRUCT()
struct FDynamicPinData
{
	GENERATED_BODY()

	UPROPERTY()
	FName PinName;

	UPROPERTY()
	FEdGraphPinType PinType;
};

UCLASS()
class COMUNICATORNODES_API UK2_SendData : public UK2Node, public IK2Node_AddPinInterface
{
	GENERATED_BODY()

public:
	
	UPROPERTY()
	FEdGraphPinType DefaultPinType;
	
	UPROPERTY()
	TArray<FDynamicPinData> CachedPins;
		
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	
	virtual FText GetMenuCategory() const override;

	virtual bool IsNodePure() const override { return true; }
	
	virtual void NotifyPinConnectionListChanged(UEdGraphPin* Pin) override;
	
	virtual void AllocateDefaultPins() override;

	virtual void GetNodeContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;

	virtual void PostEditUndo() override;

	virtual void PostLoad() override;
	
	//interface functions
	
	virtual bool CanAddPin() const override { return true; }

	virtual void AddInputPin() override;

	virtual void RemoveInputPin(UEdGraphPin* Pin) override;
};
