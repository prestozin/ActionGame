
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ACTIONGAME_API IInteractionInterface
{
	GENERATED_BODY()

	
public:
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void GetItemData(FName& OutItemName, int32& OutQuantity);
};
