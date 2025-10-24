#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/Inv_IInventoryListener.h"
#include "Interfaces/Inv_IInventoryInfo.h"
#include "Interfaces/Inv_IInventoryActions.h"
#include "Data/Inv_ItemDataStructs.h"
#include "InventoryHUD.generated.h"

#pragma region Classes

class UCanvasPanel;
class UGridPanel;
class UInventoryComponent;
class UInv_ItemInteractor;
class UInv_ItemInspector;
class UInv_OnDragSlot;
class UInv_SplitStack;
class UInv_ItemSlot;
class UInv_DragDrop;
class UInv_DropZone;
class UInv_ContextMenu;

#pragma endregion

UCLASS()
class INVENTORY_API UInventoryHUD : public UUserWidget, public IInv_IInventoryListener, public IInv_IInventoryInfo, public IInv_IInventoryActions
{
	GENERATED_BODY()
	
protected:
	
	virtual void NativeConstruct() override;
	
private:

	virtual void OnInventoryUpdate_Implementation(EInventoryUpdateType UpdateType,const TArray<int32>& ModifiedIndexes) override;
	
	// ================================
	// =        TEMPLATES           =
	// ================================
	
	template <typename T>
	T* WidgetFactory(TSubclassOf<T> WidgetClass)
	{
		if (!WidgetClass) return nullptr;
		return CreateWidget<T>(GetWorld(), WidgetClass);
	}
	
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
	
	UPROPERTY(EditDefaultsOnly, Category = "Inventory", DisplayName = "ItemSlot Widget")
	TSubclassOf<UInv_ItemSlot> ItemSlotClass;
	
	UPROPERTY(EditDefaultsOnly, Category="Inventory", DisplayName = "DragSlot Widget")
	TSubclassOf<UInv_OnDragSlot> DragSlotClass;

	UPROPERTY(EditAnywhere, Category = "Inventory", DisplayName = "DragDrop Widget")
	TSubclassOf<UInv_DragDrop> DragDropClass;

#pragma endregion

#pragma region Widgets
	
	UPROPERTY(meta = (BindWidget))
	UInv_ItemInspector* ItemInspectorWidget;
	
	UPROPERTY(meta = (BindWidget))
	UInv_DropZone* DropZoneWidget;

	UPROPERTY(meta = (BindWidget))
	UInv_ContextMenu* ContextMenuWidget;

	UPROPERTY(meta = (BindWidget))
	UInv_SplitStack* SplitStackWidget;

	UPROPERTY()
	UInv_OnDragSlot* DragVisualWidget;

	UPROPERTY()
	UInv_DragDrop* DragDropWidget;

	
#pragma endregion

#pragma region InventorySetup
	
	// ================================
	// =       PROPERTIES           =
	// ================================
	
	TScriptInterface<IInv_IInventoryInfo> InventorySource;
	
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
	
	void CreateSlot(TArray<int32> IndexToUpdate);
    
	void InsertSlot(TArray<int32> IndexesToUpdate);
    	
	void RemoveSlot(TArray<int32> IndexToUpdate);

	void UpdateExistingSlot(TArray<int32> IndexToUpdate);

	FIntPoint GetGridPosition(int32 Index) const;

#pragma endregion

#pragma region CreateWidgets
	
	void SetInspectorSetup(int32 ItemIndex, FVector2D SlotPosition);

	void OnItemDropped(UDragDropOperation* InOperation, int32 DestinationIndex) const;

	void SetSplitStackWidget(int32 Index);

	void SetDragDrop();

	void SetDropZone();

	void SetContextMenu();

	void SetContextMenuSetup(int32 SlotIndex, FVector2D SlotPosition);

	void SetWidgetPosition(const UUserWidget* WidgetToMove, const FVector2D& AbsolutePosition, FVector2D Offset);

	void SetSlotSetup (UInv_ItemSlot* ItemSlot, UTexture2D* Icon, int32 Quantity, int32 Index);
	
	UDragDropOperation* CreateDragDropWidget(UInv_ItemSlot* ItemSlot);

	UInv_ItemInspector* GetItemInspector() const { return ItemInspectorWidget; }

#pragma endregion
	
	
public:
		
	// ================================
	// =       FUNCTIONS           =
	// ================================
	
	bool ToggleHUD();

	UFUNCTION(BlueprintCallable)
	void InitializeInventory(UObject* IntInventorySource);
		
};
