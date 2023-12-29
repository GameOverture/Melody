#ifndef Channel_h__
#define Channel_h__

#include "pch.h"
#include "IComponent.h"

#define LARGE_INTRO_TIME 20.0f
#define LARGE_INTROSNAPSHOT_TIME 5.0f
#define LARGE_INTRONOWPLAYING_TIME 5.0f

#define LARGE_WIDTH 800.0f
#define LARGE_HEIGHT 600.0f
#define LARGE_CYCLE_DUR 15.0f

enum ChannelType
{
	CHANNELTYPE_Music
};

class Channel : public HyEntity2d//IComponent
{
protected:
	const ChannelType	m_eCHANNEL_TYPE;
	HySprite2d			m_Static;

public:
	Channel(ChannelType eChannelType, HyEntity2d *pParent = nullptr);
	virtual ~Channel();

	void ShowStatic();
	void HideStatic();

protected:
	virtual void OnUpdate() override;
};

#endif // Channel_h__
