#include "Listener/MessageListener.h"
#include "Message/BaseMessage.h"


FOnMessageBroadcasted* UMessageListener::GetChannelDelegate(FGameplayTag Channel)
{
	if (!Channel.IsValid()) return nullptr;
	
	return &ChannelDelegates.FindOrAdd(Channel);
}

void UMessageListener::SendMessage(FGameplayTag Channel, const FText& Text)
{
	if (!Channel.IsValid() ) return;
	
	UBaseMessage* LastMessage = NewObject<UBaseMessage>(this);
	if (!LastMessage) return;
	
	LastMessage->MessageText = Text;
	MessagesByChannel.FindOrAdd(Channel) = LastMessage;
	
	if (FOnMessageBroadcasted* Delegate = ChannelDelegates.Find(Channel))
	{
		Delegate->Broadcast(Channel, LastMessage);
	}
}



