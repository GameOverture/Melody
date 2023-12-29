#ifndef Channel_h__
#define Channel_h__

#include "pch.h"
#include "IComponent.h"

class Channel : public IComponent
{
protected:
	HySprite2d		m_Static;

public:
	Channel(ComponentType eComponentType, HyEntity2d *pParent = nullptr);
	virtual ~Channel();

	void ShowStatic();
	void HideStatic();

protected:
	virtual void OnUpdate() override;
};

#endif // Channel_h__
