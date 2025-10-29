#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Subsystem/ComunicatorSubsystem.h"
#include "AsyncListener.generated.h"

/**
 * 
 */
UCLASS()
class COMUNICATOR_API UAsyncListener : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnMessageBroadcasted OnMessageReceived;

	UFUNCTION(BlueprintCallable, DisplayName="Listen For Message (Async)", meta=(WorldContext="WorldContextObject"))
	static UAsyncListener* ListenForMessageAsync(UObject* WorldContextObject, FGameplayTag FromChannel);

protected:
	
	UPROPERTY()
	UObject* WorldContextObject;

	UPROPERTY()
	FGameplayTag TargetChannel;

	UPROPERTY()
	UComunicatorSubsystem* CachedListener;

	UFUNCTION()
	void HandleMessage(FGameplayTag Channel, UBaseMessage* Message);

	virtual void Activate() override;
};
