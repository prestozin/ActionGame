
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

	if (!DataTable) return;
	
	FItemData* Item = DataTable->FindRow<FItemData>(RowName, TEXT("GetItemFromDataTable"));

	//create a copy of the entry item and set the quantity
	FItemData ItemToAdd = *Item;
	ItemToAdd.ItemNumericData.Quantity = Quantity;

	StackItem(&ItemToAdd);

	//create a new index to each new item in the inventory
	for (int32 ItemIndex = 0; ItemIndex < Inventory.Num(); ItemIndex++)
	{
		//create a copy of the current item of the slot index in inventory
		const FItemData& CurrentItem = Inventory[ItemIndex];

		//check if the current item in the slot and the new item are qual, if not, check if the quantity of current item is bigger than 0
		if (CurrentItem.ID == ItemToAdd.ID && CurrentItem.ItemNumericData.Quantity >0)
		{
			if (!InventoryHUD->InventorySlots.IsValidIndex(ItemIndex))
			{
				InventoryHUD->CreateItemSlot(ItemIndex);
			}
			InventoryHUD->UpdateHud(ItemIndex);
		}
	}
	
		
		UE_LOG(LogTemp, Warning, TEXT("Item %s adicionado ao inventário!"), *RowName.ToString());
		
	}

void UInventoryComponent::StackItem(FItemData* Item)
{
					bool bItemFullyStacked = false;
	        		FItemData ItemToAdd = *Item;
	
                        	if (ItemToAdd.ItemNumericData.IsStackable)
                        	{
                        		for (FItemData& ExistingItem : Inventory)
                        		{
                        			
                        			if (ExistingItem.ID == ItemToAdd.ID)
                        			{
                        				//get the quantity left between max quantity - atual quantity
                        				const int32 SpaceLeft =  ExistingItem.ItemNumericData.MaxQuantity - ExistingItem.ItemNumericData.Quantity;
                        
                        				//check if the space left between the previous two values is bigger than 0
                        				if (SpaceLeft > 0) 
                        				{
                        					//if still have space left, set the bool stack
                        					bItemFullyStacked = false;
                        					
                        					//create a variable to keep the value to add to current quantity
                                         	const int32 AmountToAdd = FMath::Min(SpaceLeft, ItemToAdd.ItemNumericData.Quantity);
                        
                        					//sum these two values (Item quantity + quantity to add)
                        					ExistingItem.ItemNumericData.Quantity += AmountToAdd;
                
                        					//get the item to add quantity, and subtract the quantity that was added to inventory
                        					ItemToAdd.ItemNumericData.Quantity -= AmountToAdd;
                        					
                        					//if the quantity left to add is less or equal than 0, stop the stack
                        					if (ItemToAdd.ItemNumericData.Quantity <= 0)
                        					{
                        						bItemFullyStacked = true;
                        					}
                        				
                        				}
                        			}
                        		}
                        	}
                        
                        	//if the quantity left is bigger than 0, create a new slot with this item
                        	while (!bItemFullyStacked && ItemToAdd.ItemNumericData.Quantity > 0)
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

void UInventoryComponent::SwapItemSlot(int32 SourceIndex, int32 DestinationIndex)
{
	if (Inventory.IsValidIndex(SourceIndex) && Inventory.IsValidIndex(DestinationIndex))
	{

		FItemData& DraggedSlot = Inventory[SourceIndex];
		FItemData& DestinationSlot = Inventory[DestinationIndex];

			if (DraggedSlot.ID.IsValid() && DestinationSlot.ID.IsValid() && DraggedSlot.ID == DestinationSlot.ID)
			{
				if (DraggedSlot.ItemNumericData.IsStackable && DestinationSlot.ItemNumericData.IsStackable)
				{
					const int32 SpaceFree =  DestinationSlot.ItemNumericData.MaxQuantity - DestinationSlot.ItemNumericData.Quantity;
					const int32 AmountToAdd = FMath::Min(SpaceFree, DraggedSlot.ItemNumericData.Quantity);
                    				
					if (SpaceFree > 0)
					{
						DraggedSlot.ItemNumericData.Quantity -= AmountToAdd;
						DestinationSlot.ItemNumericData.Quantity += AmountToAdd;
                    
						if (DraggedSlot.ItemNumericData.Quantity <= 0)
						{
							Inventory.RemoveAt(SourceIndex);
						}
					}
				}
				               
				//update hud
				if (InventoryHUD)
				{
					
					InventoryHUD->UpdateHud(SourceIndex);
					InventoryHUD->UpdateHud(DestinationIndex);
				}
				UE_LOG(LogTemp, Warning, TEXT("Before Swap: %d=%s | %d=%s"), 
				SourceIndex, *Inventory[SourceIndex].ID.ToString(),
				DestinationIndex, *Inventory[DestinationIndex].ID.ToString());
			}
                
			else
			{
				Inventory.Swap(SourceIndex, DestinationIndex);
                UE_LOG(LogTemp, Warning, TEXT("After Swap: %d=%s | %d=%s"), 
                SourceIndex, *Inventory[SourceIndex].ID.ToString(),
                DestinationIndex, *Inventory[DestinationIndex].ID.ToString());					
				if (InventoryHUD)
				{
					InventoryHUD->UpdateHud(SourceIndex);
					InventoryHUD->UpdateHud(DestinationIndex);
				}
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

	











