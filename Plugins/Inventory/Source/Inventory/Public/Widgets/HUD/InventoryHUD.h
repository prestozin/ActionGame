#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/Inv_InteractionInterface.h"
#include "Data/Inv_ItemDataStructs.h"
#include "InventoryHUD.generated.h"

#pragma region Classes

class UGridPanel;
class UInventoryComponent;
class UInv_ItemInteractor;
class UInv_ItemInspector;
class UInv_OnDragSlot;
class UInv_SplitStack;
class UInv_ItemSlot;
class UInv_DragDrop;
class UInv_DropZone;
class UInv_SlotContextMenu;
enum class EInventoryUpdate : uint8;

#pragma endregion

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

	UPROPERTY(EditDefaultsOnly, Category = "Inventory", DisplayName = "ItemSlot Widget")
	TSubclassOf<UInv_ItemSlot> ItemSlotClass;
	
	UPROPERTY(EditDefaultsOnly, Category="Inventory", DisplayName = "DragSlot Widget")
	TSubclassOf<UInv_OnDragSlot> DragSlotClass;

	UPROPERTY(EditAnywhere, Category = "Inventory", DisplayName = "DragDrop Widget")
	TSubclassOf<UInv_DragDrop> DragDropClass;
	
	UPROPERTY(EditAnywhere, Category = "Inventory", DisplayName = "ItemInspector Widget")
	TSubclassOf<UInv_ItemInspector> ItemInspectorClass;
	
	UPROPERTY(EditAnywhere, Category = "Inventory", DisplayName = "SplitStack Widget")
	TSubclassOf<UInv_SplitStack> SplitStackClass;

	UPROPERTY(EditAnywhere, Category = "Inventory", DisplayName = "ContextMenu Widget")
	TSubclassOf<UInv_SlotContextMenu> ContextMenuClass;
	
#pragma endregion

#pragma region Widgets
	
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

	UPROPERTY(meta = (BindWidget))
	UInv_SlotContextMenu* ContextMenuWidget;
	
#pragma endregion

#pragma region InventorySetup
	
	// ================================
	// =       PROPERTIES           =
	// ================================
	
	UPROPERTY()
	UInventoryComponent* PlayerInventory;
	
	UPROPERTY(meta = (BindWidget))
	UGridPanel* InventoryGridPanel;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	int32 SlotsPerLine = 5;
	
	UPROPERTY()
	EItemType InventoryFilter = EItemType::None;

	UPROPERTY()
	TArray<UInv_ItemSlot*> InventorySlots;
	
	// ================================
	// =       FUNCTIONS           =
	// ================================
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void FilterInventory(EItemType FilterType);

	void UpdateIndexes();

#pragma endregion
	
	// ================================
	// =       FUNCTIONS           =
	// ================================

#pragma region SlotsSection
	
	void CreateSlot(int32 IndexToUpdate);
    
	void InsertSlot(TArray<int32> IndexesToUpdate);
    	
	void RemoveSlot(int32 IndexToRemove);

	void UpdateExistingSlot(int32 IndexToUpdate);

	FIntPoint GetGridPosition(int32 Index) const;

#pragma endregion

#pragma region CreateWidgets
	
	void CreateDefaultWidgets();

	void SetInspectorSetup(int32 ItemIndex);

	void OnItemDropped(UDragDropOperation* InOperation, int32 DestinationIndex) const;

	void CreateItemInspectorWidget();

	void CreateInteractWidget();

	void CreateSplitStackWidget(int32 Index);

	void CreateDragDropSetup();

	void CreateContextMenu();

	void SetContextMenuSetup(int32 SlotIndex);

	void SetSlotSetup (UInv_ItemSlot* ItemSlot, UTexture2D* Icon, int32 Quantity, int32 Index);
	
	UDragDropOperation* CreateDragDropWidget(UInv_ItemSlot* ItemSlot);

	UInv_ItemInspector* GetItemInspector() const { return ItemInspector; }

#pragma endregion
	
	
public:
	
	// ================================
	// =       FUNCTIONS           =
	// ================================
	
	bool ToggleHUD();
		
	UInv_ItemInteractor* GetInteractWidget() const { return InteractWidget; }

	void InitializeHUD(UInventoryComponent* Inventory);

	void HandleInventoryUpdate(EInventoryUpdate UpdateType, const TArray<int32>& ModifiedIndexes);
	
};
