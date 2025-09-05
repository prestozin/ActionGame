
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Inv_InteractionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UInv_InteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INVENTORY_API IInv_InteractionInterface
{
	GENERATED_BODY()

	
public:
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void GetItemData(FName& OutItemName, int32& OutQuantity);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	UStaticMesh* GetItemMesh(FName RowName);
};
