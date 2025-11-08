#pragma once

#include "CoreMinimal.h"
#include "DynamicPinData.generated.h"

USTRUCT()
struct FDynamicPinData
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	FName PinName;

	UPROPERTY()
	FEdGraphPinType PinType;

	UPROPERTY()
	FGuid PinGuid;
};
