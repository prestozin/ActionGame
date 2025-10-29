#include "Message/MessageAtributtes.h"
#include "KismetCompiler.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "EdGraphSchema_K2.h"

void UMessageAtributtes::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Name, TEXT("Channel"));
	for (int32 i = 0; i < NumInputs; i++)
	{
		FString PinName = FString::Printf(TEXT("Variable %d"), i + 1);
		CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Wildcard, *PinName);
	}
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, TEXT("Then"));
}

void UMessageAtributtes::AddInputPin()
{
	Modify(); 
	NumInputs++;
	ReconstructNode();
}

FText UMessageAtributtes::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(TEXT("Listen For Message (Event)"));
}

void UMessageAtributtes::ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	Super::ExpandNode(CompilerContext, SourceGraph);
}

void UMessageAtributtes::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegister) const
{
	UClass* ActionKey = GetClass();

	if (ActionRegister.IsOpenForRegistration(ActionKey))
	{
		UBlueprintNodeSpawner* NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
		ActionRegister.AddBlueprintAction(ActionKey, NodeSpawner);
	}
}

FText UMessageAtributtes::GetMenuCategory() const
{
	return FText::FromString(TEXT("Message System"));
}

void UMessageAtributtes::GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context)
{
	Super::GetNodeContextMenuActions(Menu, Context);

	if (Context->Pin) return; 

	FToolMenuSection& Section = Menu->AddSection("MessageAtributtes", FText::FromString("Message Node"));

	Section.AddMenuEntry(
		"AddInputPin",
		FText::FromString("Add Variable Pin"),
		FText::FromString("Adds a new input variable pin."),
		FSlateIcon(),
		FUIAction(
			FExecuteAction::CreateUObject(const_cast<UMessageAtributtes*>(this), &UMessageAtributtes::AddInputPin)
		)
	);
}