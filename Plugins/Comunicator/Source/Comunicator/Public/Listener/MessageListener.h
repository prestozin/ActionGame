#pragma once

#include "CoreMinimal.h"
#include "GameplayTags.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MessageListener.generated.h"


class UBaseMessage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMessageBroadcasted, FGameplayTag, Channel, UBaseMessage*, Message);

UCLASS()
class COMUNICATOR_API UMessageListener : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:
	
	UPROPERTY()
	TMap<FGameplayTag, FOnMessageBroadcasted> ChannelDelegates;
	
	UPROPERTY()
	TMap<FGameplayTag, TObjectPtr<UBaseMessage>> MessagesByChannel;
	
public:

	FOnMessageBroadcasted* GetChannelDelegate(FGameplayTag Channel);
	
	UFUNCTION(BlueprintCallable, Category="Message")
	void SendMessage(FGameplayTag Channel, const FText& Text);
};
