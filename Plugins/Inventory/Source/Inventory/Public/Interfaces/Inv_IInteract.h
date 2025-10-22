
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Inv_IInteract.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UInv_IInteract : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INVENTORY_API IInv_IInteract
{
	GENERATED_BODY()

	
public:
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void GetItemData(FName& OutItemName, int32& OutQuantity);
};
