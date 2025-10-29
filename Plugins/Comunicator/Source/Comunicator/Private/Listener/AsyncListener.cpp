#include "Listener/AsyncListener.h"
#include "Kismet/GameplayStatics.h"

UAsyncListener* UAsyncListener::ListenForMessageAsync(UObject* WorldContextObject, FGameplayTag FromChannel)
{
	UAsyncListener* GetMessage = NewObject<UAsyncListener>();
	GetMessage->WorldContextObject = WorldContextObject;
	GetMessage->TargetChannel = FromChannel;
	return GetMessage;
}

void UAsyncListener::HandleMessage(FGameplayTag Channel, UBaseMessage* Message)
{
	if (Channel == TargetChannel)
	{
		OnMessageReceived.Broadcast(Channel, Message);
		SetReadyToDestroy();
	}
}

void UAsyncListener::Activate()
{
	if (!WorldContextObject)
	{
		SetReadyToDestroy();
		return;
	}

	if (UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject))
	{
		CachedListener = GameInstance->GetSubsystem<UComunicatorSubsystem>();
	}

	if (!CachedListener || !TargetChannel.IsValid())
	{
		SetReadyToDestroy();
	}

	if (FOnMessageBroadcasted* MessageDelegate = CachedListener->GetChannelDelegate(TargetChannel))
	{
		MessageDelegate->AddDynamic(this, &UAsyncListener::HandleMessage);
	}
}