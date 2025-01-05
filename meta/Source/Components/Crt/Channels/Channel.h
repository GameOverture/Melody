#ifndef Channel_h__
#define Channel_h__

#include "pch.h"
#include "IComponent.h"

enum ChannelType
{
	CHANNELTYPE_Static = 0,
	CHANNELTYPE_Music,
	CHANNELTYPE_Game,

	NUM_CHANNELTYPE
};

class Channel : public HyEntity2d
{
protected:
	const ChannelType	m_eCHANNEL_TYPE;

public:
	Channel(ChannelType eChannelType, HyEntity2d *pParent = nullptr);
	virtual ~Channel();

protected:
	virtual void OnUpdate() override;
};

#endif // Channel_h__
