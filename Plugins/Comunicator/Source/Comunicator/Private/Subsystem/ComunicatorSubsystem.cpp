#include "Subsystem/ComunicatorSubsystem.h"
#include "Message/BaseMessage.h"


FOnMessageBroadcasted* UComunicatorSubsystem::GetChannelDelegate(FGameplayTag Channel)
{
	if (!Channel.IsValid()) return nullptr;
	
	return &ChannelDelegates.FindOrAdd(Channel);
}

void UComunicatorSubsystem::SendMessage(FGameplayTag Channel, const FText& Text)
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

void UComunicatorSubsystem::HandleMessageReceivedEvent(FGameplayTag Channel, UBaseMessage* Message)
{
	if (!Channel.IsValid() ) return;
	
	if (FOnMessageReceivedEvent* Delegate = Callbacks.Find(Channel))
	{
		if (Delegate->IsBound())
		{
			Delegate->Execute(Channel, Message);
		}
	}
}
UBaseMessage* UComunicatorSubsystem::ListenForMessageEvent(FGameplayTag FromChannel, FOnMessageReceivedEvent OnMessageReceivedEvent)
{
	if (!FromChannel.IsValid() ) return nullptr;

	FOnMessageBroadcasted& Delegate = ChannelDelegates.FindOrAdd(FromChannel);
	Delegate.AddDynamic(this, &UComunicatorSubsystem::HandleMessageReceivedEvent);
	
	Callbacks.Add(FromChannel, OnMessageReceivedEvent);
	
	UBaseMessage* LastMessage = MessagesByChannel.FindOrAdd(FromChannel);
	if (!LastMessage) return nullptr;
	
	return LastMessage;
}





