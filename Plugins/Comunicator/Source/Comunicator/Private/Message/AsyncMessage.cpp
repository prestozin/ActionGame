
#include "Message/AsyncMessage.h"
#include "Kismet/GameplayStatics.h"

UAsyncMessage* UAsyncMessage::ListenForMessage(UObject* WorldContextObject, FGameplayTag FromChannel)
{
	UAsyncMessage* GetMessage = NewObject<UAsyncMessage>();
	GetMessage->WorldContextObject = WorldContextObject;
	GetMessage->TargetChannel = FromChannel;
	return GetMessage;
}

void UAsyncMessage::HandleMessage(FGameplayTag Channel, UBaseMessage* Message)
{
	if (Channel == TargetChannel)
	{
		OnMessageReceived.Broadcast(Channel, Message);
		SetReadyToDestroy();
	}
}

void UAsyncMessage::Activate()
{
	if (!WorldContextObject)
	{
		SetReadyToDestroy();
		return;
	}

	if (UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject))
	{
		CachedListener = GameInstance->GetSubsystem<UMessageListener>();
	}

	if (!CachedListener || !TargetChannel.IsValid())
	{
		SetReadyToDestroy();
	}

	if (FOnMessageBroadcasted* MessageDelegate = CachedListener->GetChannelDelegate(TargetChannel))
	{
		MessageDelegate->AddDynamic(this, &UAsyncMessage::HandleMessage);
	}
}
