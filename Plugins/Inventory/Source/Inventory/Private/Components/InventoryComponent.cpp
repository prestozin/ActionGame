
#include "Components/InventoryComponent.h"

#include "Widgets/HUD/InventoryHUD.h"
#include "Data/Inv_ItemDataStructs.h"
#include "Widgets/Item/Inv_InteractWidget.h"


UInventoryComponent::UInventoryComponent()
{
	
	PrimaryComponentTick.bCanEverTick = false;
}


void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	//create necessary data
	CreateDefaults();

}
void UInventoryComponent::CreateDefaults()
{
		//Get owning controller
    	OwningController = GetWorld()->GetFirstPlayerController();
	
    	//create interact widget
    	InteractWidget = CreateWidget<UInv_InteractWidget>(GetWorld(), InteractWidgetClass);

		//only add to viewport if widget is valid
    	if (InteractWidget)
    	{
    		InteractWidget->AddToViewport();
    	}

		//create hud widget
		CreateHUDWidget();
}

void UInventoryComponent::CreateHUDWidget()
{
	if (OwningController.IsValid())
	{
		InventoryHUD = CreateWidget<UInventoryHUD>(OwningController.Get(),HUDWidgetClass);
        
        	if (IsValid(InventoryHUD))
        	{
        		InventoryHUD->GetInventoryComponent(this);
        		InventoryHUD->AddToViewport();
        		InventoryHUD->SetVisibility(ESlateVisibility::Collapsed);
        	}
	}
	}



void UInventoryComponent::AddItem(FName RowName, int32 Quantity)
{
	
	FItemData* Item = DataTable->FindRow<FItemData>(RowName, TEXT("GetItemFromDataTable"));
	if (Item)
	{
		Item->ItemNumericData.Quantity = Quantity;
		StackItem(Item);
		InventoryHUD->CreateItemSlot();
		UE_LOG(LogTemp, Warning, TEXT("Item %s adicionado ao inventário!"), *RowName.ToString());
		
	}

	else
	{
		UE_LOG(LogTemp, Error, TEXT("Item %s não encontrado na DataTable!"), *RowName.ToString());
	}
}

void UInventoryComponent::StackItem(FItemData* Item)
{
	//check if item is not null
	if (Item)
	{
		//create a copy of item to add
        	FItemData ItemToAdd = *Item;
        	
        	//check if the item is stackable
        	if (ItemToAdd.ItemNumericData.IsStackable)
        	{
        		for (FItemData& ExistingItem : Inventory)
        		{
        			//check if already exist this kind of item in inventory
        			if (ExistingItem.ID == ItemToAdd.ID)
        			{
        				//get the quantity left between max quantity - atual quantity
        				const int32 SpaceLeft =  ExistingItem.ItemNumericData.MaxQuantity - ExistingItem.ItemNumericData.Quantity;
        
        				//check if the space left between the previous two values is bigger than 0
        				if (SpaceLeft > 0)
        				{
        					//create a variable to keep the value to add to current quantity
                         	const int32 ToAdd = FMath::Min(SpaceLeft, ItemToAdd.ItemNumericData.Quantity);
        
        					//sum these two values (Item quantity + quantity to add)
        					ExistingItem.ItemNumericData.Quantity += ToAdd;

        					//get the item to add quantity, and subtract the quantity that was added to inventory
        					ItemToAdd.ItemNumericData.Quantity -= ToAdd;
        					
        					//if the quantity left to add is less or equal than 0, stop the loop
        					if (ItemToAdd.ItemNumericData.Quantity <= 0) break;
        				
        				}
        			}
        		}
        	}
        
        	//if the quantity left if bigger than 0, create a new slot with this itemz
        	while (ItemToAdd.ItemNumericData.Quantity >0)
        		{

        		    //get the maximum quantity that a new stack can have
        			int32 NewStack = FMath::Min(ItemToAdd.ItemNumericData.Quantity, ItemToAdd.ItemNumericData.MaxQuantity);

        			//create a copy of item to add to not change the new stack value
        			FItemData NewItemStack = ItemToAdd;

        			//modify the value of this item to use the maximum quantity left to stack
        			NewItemStack.ItemNumericData.Quantity = NewStack;

        			//create a new slot with this maximum quantity left
        			Inventory.Add(NewItemStack);

        			//update the value of item to add to be the actual quantity - maximum quantity left in stack (new stack)
        			ItemToAdd.ItemNumericData.Quantity -= NewStack;

				}
	}
}

UStaticMesh* UInventoryComponent::GetItemMesh_Implementation(FName RowName)
{
	if (!DataTable)  return nullptr;
	
	FItemData* Item = DataTable->FindRow<FItemData>(RowName, TEXT("GetItemFromDataTable"));

	if (Item)
	{
		return Item->ItemAssetData.Mesh.LoadSynchronous();
	}
	return nullptr;
}









