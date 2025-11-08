#include "MessageData/K2_GetData.h"

#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Message/BaseMessage.h"

UK2_GetData::UK2_GetData()
{
	FEdGraphPinType WildcardType;
	const UEdGraphSchema_K2* Schema = GetDefault<UEdGraphSchema_K2>();
	WildcardType.PinCategory = Schema->PC_Wildcard;
	DefaultOutputType = WildcardType;
}

void UK2_GetData::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	UClass* ActionKey = GetClass();
	if (ActionRegistrar.IsOpenForRegistration(ActionKey))
	{
		UBlueprintNodeSpawner* NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
		check(NodeSpawner);

		ActionRegistrar.AddBlueprintAction(ActionKey, NodeSpawner);
	}
}

FText UK2_GetData::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString("Get Data");
}

FText UK2_GetData::GetMenuCategory() const
{
	return FText::FromString("Communicator");
}

void UK2_GetData::RecountVariables()
{
	int32 Index = 1;
	for (FDynamicPinPair& Pair : PinPairs)
	{
		UEdGraphPin* InPin = FindPinById(Pair.InputPinID);
		UEdGraphPin* OutPin = FindPinById(Pair.OutputPinID);

		if (InPin)
		{
			InPin->PinName = FName(*FString::Printf(TEXT("Variable_%d_Name"), Index));
		}
		if (OutPin)
		{
			OutPin->PinName = FName(*FString::Printf(TEXT("Variable_%d_Value"), Index));
		}
		
		// update cached
		for (FDynamicPinData& Data : CachedPins)
		{
			if (Data.PinGuid == Pair.InputPinID)
			{
				Data.PinName = InPin->PinName;
			}
				
			if (Data.PinGuid == Pair.OutputPinID)
			{
				Data.PinName = OutPin->PinName;
			}
		}
		Index++;
	}
	LastVariableIndex = Index - 1;
}

void UK2_GetData::AllocateDefaultPins()
{
	const UEdGraphSchema_K2* Schema = GetDefault<UEdGraphSchema_K2>();
	
	CreatePin(EGPD_Input, Schema->PC_Name, TEXT("Variable Name"));
	CreatePin(EGPD_Output, DefaultOutputType, TEXT("Variable Value"));
	
	CreatePin(EGPD_Input, Schema->PC_Object, UBaseMessage::StaticClass(), TEXT("Message"));
	
	for (FDynamicPinData Pin : CachedPins)
	{
		FString PinName = Pin.PinName.ToString();
		
		if (PinName.EndsWith("Name"))
		{
			UEdGraphPin* NewPin = CreatePin(EGPD_Input, Pin.PinType, Pin.PinName);
			NewPin->PinId = Pin.PinGuid; 
		}
		else if (PinName.EndsWith("Value"))
		{
			UEdGraphPin* NewPin = CreatePin(EGPD_Output, Pin.PinType, Pin.PinName);
			NewPin->PinId = Pin.PinGuid; 
		}
	}
	ReorderPins();
}

void UK2_GetData::NotifyPinConnectionListChanged(UEdGraphPin* Pin)
{
	if (!Pin || Pin->Direction != EGPD_Output) return;
	
	if (Pin->LinkedTo.Num() <= 0)
	{
		
		FEdGraphPinType WildType;
		const UEdGraphSchema_K2* Schema = GetDefault<UEdGraphSchema_K2>();
		WildType.PinCategory = Schema->PC_Wildcard;
		Pin->PinType = WildType;
		
		if (Pin->PinName == TEXT("Variable Value"))
		{
			DefaultOutputType = WildType;
		}
		
		for (FDynamicPinData& Data : CachedPins)
		{
			if (Pin->PinId == Data.PinGuid)
			{
				Data.PinType = WildType;
			}
		}
		return;
	}
	
	if (UEdGraphPin* ConnectedPin = Pin->LinkedTo[0])
	{
		if (Pin->PinName == TEXT("Variable Value"))
		{
			DefaultOutputType = ConnectedPin->PinType;
		}
		Pin->PinType = ConnectedPin->PinType;
		for (FDynamicPinData& Data : CachedPins)
		{
			if (Pin->PinId == Data.PinGuid)
			{
				Data.PinType = ConnectedPin->PinType;
			}
		}
	}
}

void UK2_GetData::GetNodeContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const
{
	FToolMenuSection& Section = Menu->FindOrAddSection("Message Attributes", FText::FromString(TEXT("Pin Actions")));

	if (Context->Node)
	{
		Section.AddMenuEntry
		(
			"Add Pin",
			FText::FromString("Add Input Pin"),
			FText::FromString("Add a new input pin"),
			FSlateIcon(),
			FUIAction(FExecuteAction::CreateUObject(const_cast<UK2_GetData*>(this), &UK2_GetData::AddInputPin))
		);
	}
	if (Context->Pin &&
		!(Context->Pin->PinName == TEXT("Variable Name") ||
		Context->Pin->PinName == TEXT("Variable Value") ||
		Context->Pin->PinName == TEXT("Message")
		)) 
	{
		int32 PinCount = 0;
		for (UEdGraphPin* Pin : Pins)
		{
			if (Pin->Direction == EGPD_Input)
				PinCount++;
		}
		
		if (PinCount > 2) 
		{
			Section.AddMenuEntry	
			(
		 "RemovePin",
		  FText::FromString("Remove Pin"),
		 FText::FromString("Remove this input pin"),
				 FSlateIcon(),
		FUIAction(FExecuteAction::CreateUObject(const_cast<UK2_GetData*>(this), &UK2_GetData::RemoveInputPin, const_cast<UEdGraphPin*>(Context->Pin)))
			);
		}
	}
}

void UK2_GetData::AddInputPin()
{
	FDynamicPinData PinData;
		
	FName NewPinInputName = FName(*FString::Printf(TEXT("Variable_%d_Name"), LastVariableIndex + 1));
	FName NewPinOutputName = FName(FString::Printf(TEXT("Variable_%d_Value"), LastVariableIndex + 1));
	
	const UEdGraphSchema_K2* Schema = GetDefault<UEdGraphSchema_K2>();
	
	Modify();
	
	UEdGraphPin* NewInputPin = CreatePin(EGPD_Input, Schema->PC_Name, FName(NewPinInputName));
	PinData.PinName = NewInputPin->PinName;
	PinData.PinType	= NewInputPin->PinType;
	PinData.PinGuid = NewInputPin->PinId;
	CachedPins.Add(PinData);
	
	
	UEdGraphPin* NewOutputPin = CreatePin(EGPD_Output, Schema->PC_Wildcard, FName(NewPinOutputName));
	PinData.PinName = NewOutputPin->PinName;
	PinData.PinType	= NewOutputPin->PinType;
	PinData.PinGuid = NewOutputPin->PinId;
	CachedPins.Add(PinData);

	FDynamicPinPair PinPair;
	PinPair.InputPinID = NewInputPin->PinId;
	PinPair.OutputPinID = NewOutputPin->PinId;
	PinPairs.Add(PinPair);
	
	LastVariableIndex++;
	
	ReorderPins();
	GetGraph()->NotifyNodeChanged(this);
	FBlueprintEditorUtils::MarkBlueprintAsModified(GetBlueprint());
}

void UK2_GetData::RemoveInputPin(UEdGraphPin* Pin)
{
	if (Pin->PinName == TEXT("Message") || Pin->PinName == TEXT("Variable Name") || Pin->PinName == TEXT("Variable Value")) return;
	
	Pin->Modify();

	int32 PairIndex = PinPairs.IndexOfByPredicate([&] (const FDynamicPinPair& Pair)  //lambda function to get the index of the pin pair
	{
		return Pair.InputPinID == Pin->PinId || Pair.OutputPinID == Pin->PinId;
	});
	
	if (PairIndex == INDEX_NONE) return;

	FGuid InputId = PinPairs[PairIndex].InputPinID;
	FGuid OutputId = PinPairs[PairIndex].OutputPinID;
	
	RemovePinPairs({InputId, OutputId});
	
	PinPairs.RemoveAt(PairIndex);

	RecountVariables();
	
	GetGraph()->NotifyNodeChanged(this);
	FBlueprintEditorUtils::MarkBlueprintAsModified(GetBlueprint());
}

void UK2_GetData::RemovePinPairs(const TArray<FGuid>& PinIds)
{
	for (const FGuid& PinId : PinIds)
	{
		UEdGraphPin* PinToRemove = nullptr;
		for (UEdGraphPin* Pin : Pins)
		{
			if (Pin && Pin->PinId == PinId)
			{
				PinToRemove = Pin;
			}
		}

		if (PinToRemove)
		{
			PinToRemove->BreakAllPinLinks();
			PinToRemove->Modify();
			Pins.Remove(PinToRemove);
			PinToRemove->MarkAsGarbage();

			CachedPins.RemoveAll([&](const FDynamicPinData& Data)
			{
				return Data.PinName == PinToRemove->PinName;
			});
		}
	}
}

void UK2_GetData::ReorderPins()
{
	//remove all pins from pin array,
	//then put every pin that is not the ubasemessage pin in array again
	//then put the ubasemessage pin in the last position to keep visual order
	
	UEdGraphPin* MessagePin = nullptr;
	TArray<UEdGraphPin*> OtherPins;

	for (UEdGraphPin* Pin : Pins)
	{
		if (Pin->PinName == TEXT("Message"))
		{
			MessagePin = Pin;
		}
		else
		{
			OtherPins.Add(Pin);
		}
	}
	
	Pins = OtherPins;

	if (MessagePin)
	{
		Pins.Add(MessagePin);
	}
}

void UK2_GetData::PostLoad()
{
	Super::PostLoad();
}

void UK2_GetData::PostEditUndo()
{
	Super::PostEditUndo();
}



