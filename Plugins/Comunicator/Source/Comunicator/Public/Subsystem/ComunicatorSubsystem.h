#pragma once

#include "CoreMinimal.h"
#include "GameplayTags.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ComunicatorSubsystem.generated.h"


class UBaseMessage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMessageBroadcasted, FGameplayTag, Channel, UBaseMessage*, Message);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnMessageReceivedEvent, FGameplayTag, Channel, UBaseMessage*, Message);

UCLASS()
class COMUNICATOR_API UComunicatorSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:
	
	UPROPERTY()
	TMap<FGameplayTag, FOnMessageBroadcasted> ChannelDelegates;

	UPROPERTY()
	TMap<FGameplayTag, FOnMessageReceivedEvent> Callbacks;
	
	UPROPERTY()
	TMap<FGameplayTag, TObjectPtr<UBaseMessage>> MessagesByChannel;
	
	UFUNCTION()
	void HandleMessageReceivedEvent(FGameplayTag Channel, UBaseMessage* Message);
	
public:
	
	FOnMessageBroadcasted* GetChannelDelegate(FGameplayTag Channel);
	
	UFUNCTION(BlueprintCallable, Category="Message")
	void SendMessage(FGameplayTag Channel, const FText& Text);
	
	UFUNCTION(BlueprintCallable, Category = "Message", DisplayName="Listen For Message (Event)")
	UBaseMessage* ListenForMessageEvent(FGameplayTag FromChannel, FOnMessageReceivedEvent OnMessageReceivedEvent);
};
