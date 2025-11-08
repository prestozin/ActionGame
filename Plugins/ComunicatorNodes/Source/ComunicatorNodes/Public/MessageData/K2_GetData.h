// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Structs/DynamicPinData.h"
#include "CoreMinimal.h"
#include "K2Node.h"
#include "K2Node_AddPinInterface.h"
#include "K2_GetData.generated.h"

USTRUCT()
struct FDynamicPinPair
{
	GENERATED_BODY()

	UPROPERTY()
	FGuid InputPinID;

	UPROPERTY()
	FGuid OutputPinID;
};

UCLASS()
class COMUNICATORNODES_API UK2_GetData : public UK2Node, public IK2Node_AddPinInterface
{
	GENERATED_BODY()
protected:
	
	UK2_GetData();
	
public:
	
	UPROPERTY()
	FEdGraphPinType DefaultOutputType;
	
	UPROPERTY()
	TArray<FDynamicPinPair> PinPairs;
	
	UPROPERTY()
	int32 LastVariableIndex = 0;
	
	UPROPERTY()
	TArray<FDynamicPinData> CachedPins;

	void RemovePinPairs(const TArray<FGuid>& PinIds);
	
	void RecountVariables ();
	
	virtual bool IsNodePure() const override { return true; }

	virtual void AllocateDefaultPins() override;

	virtual void NotifyPinConnectionListChanged(UEdGraphPin* Pin) override;

	virtual void GetNodeContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;

	virtual FText GetMenuCategory() const override;
	
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	
	virtual void PostLoad() override;
	
	virtual void PostEditUndo() override;

	void ReorderPins();
	
	//interfaces
	
	virtual bool CanAddPin() const override { return true; }

	virtual void AddInputPin() override;

	virtual void RemoveInputPin(UEdGraphPin* Pin) override;
		
};
