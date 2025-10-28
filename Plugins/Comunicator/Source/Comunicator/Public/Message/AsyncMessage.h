// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Listener/MessageListener.h"
#include "AsyncMessage.generated.h"

/**
 * 
 */
UCLASS()
class COMUNICATOR_API UAsyncMessage : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnMessageBroadcasted OnMessageReceived;

	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContextObject"))
	static UAsyncMessage* ListenForMessage(UObject* WorldContextObject, FGameplayTag FromChannel);

protected:
	
	UPROPERTY()
	UObject* WorldContextObject;

	UPROPERTY()
	FGameplayTag TargetChannel;

	UPROPERTY()
	UMessageListener* CachedListener;

	UFUNCTION()
	void HandleMessage(FGameplayTag Channel, UBaseMessage* Message);

	virtual void Activate() override;
};
