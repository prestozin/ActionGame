#include "Messages/K2_SendData.h"
#include "EdGraphSchema_K2.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Message/BaseMessage.h"

void UK2_SendData::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	UClass* ActionKey = GetClass();
	if (ActionRegistrar.IsOpenForRegistration(ActionKey))
	{
		UBlueprintNodeSpawner* NodeSpawner = UBlueprintNodeSpawner::Create(GetClass()); 
		check(NodeSpawner);

		ActionRegistrar.AddBlueprintAction(ActionKey, NodeSpawner);  // add node in editor
	}
}

FText UK2_SendData::GetNodeTitle(ENodeTitleType::Type TitleType) const //set node title
{
	return FText::FromString(TEXT("Send Data")); 
}

FText UK2_SendData::GetMenuCategory() const   //set node category
{
	return FText::FromString(TEXT("Comunicator")); 
}

void UK2_SendData::AllocateDefaultPins()
{
	if (DefaultPinType.PinCategory.IsNone())
	{
		DefaultPinType = FEdGraphPinType();
		DefaultPinType.PinCategory = UEdGraphSchema_K2::PC_Wildcard;
	}
	
	const UEdGraphSchema_K2* Schema = GetDefault<UEdGraphSchema_K2>();
	
	CreatePin(EGPD_Input, DefaultPinType, TEXT("Value"));
	CreatePin(EGPD_Output, Schema->PC_Object, UBaseMessage::StaticClass(), TEXT("Output"));

	for (const FDynamicPinData& Data : CachedPins)
	{
		CreatePin(EGPD_Input, Data.PinType, Data.PinName);
	}
}

void UK2_SendData::GetNodeContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const
{
	FToolMenuSection& Section = Menu->FindOrAddSection("Message Atributtes", FText::FromString("Pin Actions"));
	
	if (Context->Node)                                         //add the option add pin to node context menu
	{
		Section.AddMenuEntry(
			"AddPin",
			FText::FromString("Add Input Pin"),
			FText::FromString("Add a new input pin"),
			FSlateIcon(),
			FUIAction(FExecuteAction::CreateUObject(const_cast<UK2_SendData*>(this), &UK2_SendData::AddInputPin))
		);
	}
	
	if (Context->Pin &&  Context->Pin->Direction == EGPD_Input)   
	{
		int32 PinCount = 0;
		for (UEdGraphPin* Pin : Pins)
		{
			if (Pin->Direction == EGPD_Input)
			PinCount++;
		}
		
		if (PinCount > 1) 
		{
			Section.AddMenuEntry								//add the option remove pin to node context menu
			(
		 "RemovePin",
		  FText::FromString("Remove Pin"),
		 FText::FromString("Remove this input pin"),
				 FSlateIcon(),
		FUIAction(FExecuteAction::CreateUObject(const_cast<UK2_SendData*>(this), &UK2_SendData::RemoveInputPin, const_cast<UEdGraphPin*>(Context->Pin)))
			);
		}
	}
}

void UK2_SendData::NotifyPinConnectionListChanged(UEdGraphPin* Pin)
{
	if (!Pin) return;
	if (Pin->Direction != EGPD_Input) return;

	const UEdGraphSchema_K2* Schema = GetDefault<UEdGraphSchema_K2>();
	
	if (Pin->LinkedTo.Num() > 0)								//when a variable is connected, convert the pin type to the type of the variable connected
	{
		if (UEdGraphPin* LinkedPin = Pin->LinkedTo[0])			
		{
			if (Pin->PinName == TEXT("Value"))
			{
				DefaultPinType = LinkedPin->PinType;
			}
			
			Pin->PinType = LinkedPin->PinType;
			
			Schema->ValidateExistingConnections(Pin);

			for (FDynamicPinData& Data : CachedPins)
			{
				if (Data.PinName == Pin->PinName)
				{
					Data.PinType = Pin->PinType;				//cache the type of all conected variables to load after on start
					break;
				}
			}
		}
	}
	else														//reset pin type after variable disconnected
	{
		if (Pin->PinName == TEXT("Value"))
		{
			DefaultPinType.PinCategory = UEdGraphSchema_K2::PC_Wildcard;
		}
		
		Pin->PinType.PinCategory = UEdGraphSchema_K2::PC_Wildcard;
		Pin->PinType.PinSubCategory = NAME_None;
		Pin->PinType.PinSubCategoryObject = nullptr;
		Pin->PinType.ContainerType = EPinContainerType::None;
		
		for (FDynamicPinData& Data : CachedPins)
		{
			if (Data.PinName == Pin->PinName)									//change the value of cached variable for wildcard after variable disconnected
			{
				Data.PinType.PinCategory = UEdGraphSchema_K2::PC_Wildcard;
				break;
			}
		}
	}
}

void UK2_SendData::AddInputPin()
{
	int32 PinIndex = 0;
	for (UEdGraphPin* Pin : Pins)
	{
		if (Pin->Direction == EGPD_Input)
		{
			FString PinName = Pin->PinName.ToString();
			if (PinName == TEXT("Value"))
			{
				PinIndex = FMath::Max(PinIndex, 1); 
			}
			else if (PinName.StartsWith(TEXT("Value_")))
			{
				FString IndexStr;
				if (PinName.Split(TEXT("_"), nullptr, &IndexStr, ESearchCase::CaseSensitive))
				{
					int32 CurrentIndex = FCString::Atoi(*IndexStr);
					PinIndex = FMath::Max(PinIndex, CurrentIndex);
				}
			}
		}
	}
	FString NewPinName = FString::Printf(TEXT("Value_%d"), PinIndex + 1);
	
	const UEdGraphSchema_K2* Schema = GetDefault<UEdGraphSchema_K2>();
		
	UEdGraphPin* NewPin = CreatePin(EGPD_Input, Schema->PC_Wildcard, FName(*NewPinName));
	
	FDynamicPinData Data;
	Data.PinName = NewPin->PinName;
	Data.PinType = NewPin->PinType;
	
	CachedPins.Add(Data);
	
	GetGraph()->NotifyNodeChanged(this);
	FBlueprintEditorUtils::MarkBlueprintAsModified(GetBlueprint());
}

void UK2_SendData::RemoveInputPin(UEdGraphPin* Pin)
{
	if (!Pin) return;
	
	CachedPins.RemoveAll([&](const FDynamicPinData& Data)
	{
		return Data.PinName == Pin->PinName;
	});
	
	int32 PinCount = 0;
	for (UEdGraphPin* ExistingPin : Pins)
	{
		if (ExistingPin->Direction == EGPD_Input)
		{
			PinCount++;
		}
	}

	if (PinCount <= 1) return;
	
	Pins.Remove(Pin);
	Pin->Modify();
	Pin->BreakAllPinLinks();
	Pin->MarkAsGarbage();
	GetGraph()->NotifyNodeChanged(this);

	FBlueprintEditorUtils::MarkBlueprintAsModified(GetBlueprint());
}

void UK2_SendData::PostEditUndo()
{
	Super::PostEditUndo();
	GetGraph()->NotifyGraphChanged();
}

void UK2_SendData::PostLoad()
{
	Super::PostLoad();
}
