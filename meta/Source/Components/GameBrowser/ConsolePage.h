#ifndef ConsolePage_h__
#define ConsolePage_h__

#include "pch.h"

class ConsolePage : public HyUiContainer
{
	std::vector<HyButton *>		m_LogoBtnList;
	std::vector<HySprite2d *>	m_LogoSpriteList;
	int32						m_iHoverIndex;

public:
	ConsolePage(HyEntity2d *pParent = nullptr);
	virtual ~ConsolePage();

protected:
	virtual void OnContainerUpdate() override;

	virtual float OnBeginShow() override;	// Returns the duration (in seconds) of the show transition
	virtual void OnShown() override;
	virtual float OnBeginHide() override; // Returns the duration (in seconds) of the hide transition
	virtual void OnHidden() override;
};

#endif // ConsolePage_h__
