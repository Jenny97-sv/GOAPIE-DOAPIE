#pragma once

enum class eMessage
{
	ePlayerDashed,
	ePlayerSmiled,
	ePlayerPet,
	ePlayerFeed,
	eNone
};

struct Message
{
	eMessage msg = eMessage::eNone;
	void* data = nullptr;
};

class Observer
{
public:
	virtual void OnMessageSent(const Message msg) = 0;
};