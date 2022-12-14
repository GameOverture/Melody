#ifndef Channel_h__
#define Channel_h__

#include "pch.h"

class Channel : public HyEntity2d
{
protected:
	HySprite2d		m_Static;

public:
	Channel(HyEntity2d *pParent = nullptr);
	virtual ~Channel();

protected:
	virtual void OnUpdate() override;
};

#endif // Channel_h__
