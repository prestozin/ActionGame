#include "Subsystem/ComunicatorSubsystem.h"
#include "Message/BaseMessage.h"



void UComunicatorSubsystem::SendMessage(FGameplayTag Channel, UBaseMessage* Message)
{
	if (!Channel.IsValid() ) return;
	
	UBaseMessage* LastMessage = NewObject<UBaseMessage>(this);
	if (!LastMessage) return;
	
	MessagesByChannel.FindOrAdd(Channel) = LastMessage;
	
	if (FOnMessageBroadcasted* Delegate = ChannelDelegates.Find(Channel))
	{
		Delegate->Broadcast(Channel, LastMessage);
	}
}

DEFINE_FUNCTION(UComunicatorSubsystem::execSendData)
{
	P_GET_PROPERTY(FNameProperty, VariableName)
	
	Stack.MostRecentProperty = nullptr;
	Stack.StepCompiledIn<FProperty>(nullptr);
	void* ValuePtr = Stack.MostRecentPropertyAddress;

	P_FINISH;
	
	FProperty* Prop = Stack.MostRecentProperty;
	if (!Prop || !ValuePtr) return;
}

UBaseMessage* UComunicatorSubsystem::SendData(FName VariableName,int32 Value)
{
	return nullptr;	
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

FOnMessageBroadcasted* UComunicatorSubsystem::GetChannelDelegate(FGameplayTag Channel)
{
	if (!Channel.IsValid()) return nullptr;
	
	return &ChannelDelegates.FindOrAdd(Channel);
}
