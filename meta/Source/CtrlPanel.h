#ifndef CtrlPanel_h__
#define CtrlPanel_h__

#include "pch.h"

class IComponent;
class Crt;
class MessageCycle;

class CtrlPanel : public HyUiContainer
{
	Crt &				m_CrtRef;
	
	HyButton			m_btnVolume_Down;
	HyButton			m_btnVolume_Up;

public:
	CtrlPanel(Crt &crtRef, HyEntity2d *pParent = nullptr);
	virtual ~CtrlPanel();
};

#endif // CtrlPanel_h__
