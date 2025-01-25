#ifndef CtrlPanel_h__
#define CtrlPanel_h__

#include "pch.h"

class IComponent;
class Crt;
class MessageCycle;

class CtrlPanel : public HyUiContainer
{
	Crt *				m_pCrtRef;
	
	HyButton			m_btnVolume_Down;
	HyButton			m_btnVolume_Up;

public:
	CtrlPanel(HyEntity2d *pParent = nullptr);
	virtual ~CtrlPanel();

	void SetCrtRef(Crt *pCrtRef);
};

#endif // CtrlPanel_h__
