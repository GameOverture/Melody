#ifndef CtrlPanel_h__
#define CtrlPanel_h__

#include "pch.h"

class CtrlPanel : public HyUiContainer
{
	std::vector<HyDividerLine *>	m_DividerLineList;

public:
	CtrlPanel(HyEntity2d *pParent = nullptr);
	virtual ~CtrlPanel();

	void InsertDividerLine();
};

#endif // CtrlPanel_h__
