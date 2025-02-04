#ifndef ConsolePage_h__
#define ConsolePage_h__

#include "pch.h"

class ConsolePage : public HyUiContainer
{
	HyLabel					m_TitleLabel;

	std::vector<HyLabel *>	m_ConsoleTitleList;
	std::vector<HyButton *>	m_ConsoleBtnList;

	int32					m_iHoverIndex;

public:
	ConsolePage(HyEntity2d *pParent = nullptr);
	virtual ~ConsolePage();

protected:
	virtual void OnContainerUpdate() override;
};

#endif // ConsolePage_h__
