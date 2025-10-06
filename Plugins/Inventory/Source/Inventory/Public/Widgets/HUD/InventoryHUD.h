#pragma once


UENUM()
enum class EHUDUpdates : uint8
{
	Create	UMETA(DisplayName = "Create"),
	Insert  UMETA(DisplayName = "Insert"),
	Remove  UMETA(DisplayName = "Remove"),
	Existing   UMETA(DisplayName = "Existing")
};



#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/Inv_InteractionInterface.h"
#include "Data/Inv_ItemDataStructs.h"
#include "InventoryHUD.generated.h"

class UGridPanel;
class UInventoryComponent;
class UInv_ItemInteractor;
class UInv_ItemInspector;
class UInv_OnDragSlot;
class UInv_SplitStack;
class UInv_ItemSlot;
class UInv_DragDrop;
class UInv_DropZone;
class UInv_Filter;




UCLASS()
class INVENTORY_API UInventoryHUD : public UUserWidget, public IInv_InteractionInterface
{
	GENERATED_BODY()
	
protected:
	
	virtual void NativeConstruct() override;
	
private:

	// ================================
	// =        TEMPLATES           =
	// ================================

	
	//convert enum to ftext
	template<typename Enums>
	FText EnumToText(Enums EnumValue)
	{
		if (const UEnum* EnumPtr = StaticEnum<Enums>())
		{
			return EnumPtr->GetDisplayNameTextByValue((int64)EnumValue);
		}
		return FText::FromString("Invalid");
	}

	// ================================
	// =        PROPERTIES            =
	// ================================

#pragma region WidgetsClass
	
	UPROPERTY(EditDefaultsOnly, Category = "Inventory", DisplayName = "Interact Widget")
	TSubclassOf<UInv_ItemInteractor> InteractWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory", DisplayName = "Item Slot Widget")
	TSubclassOf<UInv_ItemSlot> ItemSlotClass;
	
	UPROPERTY(EditDefaultsOnly, Category="Inventory", DisplayName = "DragSlot Widget")
	TSubclassOf<UInv_OnDragSlot> DragSlotClass;

	UPROPERTY(EditAnywhere, Category = "Inventory", DisplayName = "DragDrop Widget")
	TSubclassOf<UInv_DragDrop> DragDropClass;
	
	UPROPERTY(EditAnywhere, Category = "Inventory", DisplayName = "Item Inspector Widget")
	TSubclassOf<UInv_ItemInspector> ItemInspectorClass;
	
	UPROPERTY(EditAnywhere, Category = "Inventory", DisplayName = "SplitStack Widget")
	TSubclassOf<UInv_SplitStack> SplitStackClass;
	
#pragma endregion

#pragma region WidgetsBP
	
	UPROPERTY()
	UInv_ItemInspector* ItemInspector;
	
	UPROPERTY()
	UInv_ItemInteractor* InteractWidget;

	UPROPERTY()
	UInv_SplitStack* SplitStackWidget;

	UPROPERTY()
	UInv_OnDragSlot* DragVisualWidget;

	UPROPERTY()
	UInv_DragDrop* DragDropWidget;

	UPROPERTY(meta = (BindWidget))
	UInv_DropZone* DropZoneWidget;
	
#pragma endregion
	
	UPROPERTY(meta = (BindWidget))
	UGridPanel* InventoryGridPanel;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 SlotsPerLine = 5;

	UPROPERTY(meta = (BindWidget))
	UInv_Filter* FilterButton;
	
	// ================================
	// =       FUNCTIONS           =
	// ================================
    	
	void CreateSlot(TArray<int32> IndexesToUpdate);
    
	void InsertSlot(TArray<int32> IndexesToUpdate);
    	
	void RemoveSlot(TArray<int32> IndexesToUpdate);

	void UpdateExistingSlot(TArray<int32> IndexesToUpdate);

	FIntPoint GetGridPosition(int32 Index) const;

	void CreateDefaultWidgets();

	void SetInspectorSetup(int32 ItemIndex);

	void OnItemDropped(UDragDropOperation* InOperation, int32 DestinationIndex) const;

	void CreateItemInspectorWidget();

	void CreateInteractWidget();

	void CreateSplitStackWidget(int32 Index);

	void CreateDragDropSetup();

	void SetSlotSetup (UInv_ItemSlot* ItemSlot, UTexture2D* Icon, int32 Quantity, int32 Index);
	
	UDragDropOperation* CreateDragDropWidget(UInv_ItemSlot* ItemSlot);

	UInv_ItemInspector* GetItemInspector() const { return ItemInspector; }

	void SetupFilter(EItemType& EnumType);
	
public:
	
	// ================================
	// =       PROPERTIES          =
	// ================================

	UPROPERTY()
	UInventoryComponent* PlayerInventory;

	UPROPERTY()
	TArray<UInv_ItemSlot*> InventorySlots;
	
	UPROPERTY()
	bool bInventoryOpen;

	// ================================
	// =       FUNCTIONS           =
	// ================================
	
	bool ToggleHUD();
	
	void UpdateIndexes();
	
	UInv_ItemInteractor* GetInteractWidget() const { return InteractWidget; }
	
	// ================================
	// =        TEMPLATES           =
	// ================================

	template<typename... Indexes>
	void UpdateSlots(EHUDUpdates UpdateType, Indexes... ModifiedIndexes)
	{
		TArray<int32> IndexesToUpdate = { ModifiedIndexes... };

		if (!PlayerInventory) return;
		
		switch (UpdateType)
		{
		case EHUDUpdates::Create:
			CreateSlot(IndexesToUpdate);
			break;
        
		case EHUDUpdates::Remove:
			RemoveSlot(IndexesToUpdate);
			break;
        
		case EHUDUpdates::Insert:
			InsertSlot(IndexesToUpdate);
			break;

		case EHUDUpdates::Existing:
			UpdateExistingSlot(IndexesToUpdate);
			break;
		}
		UpdateIndexes();
	}
	
};
