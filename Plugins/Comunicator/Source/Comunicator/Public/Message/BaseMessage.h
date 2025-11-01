// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BaseMessage.generated.h"

UCLASS(BlueprintType)
class COMUNICATOR_API UBaseMessage : public UObject
{
	GENERATED_BODY()

private:

	UPROPERTY()
	TMap<FName, int32> VariablesContainer;

	
public:

	
};
