#include "pch.h"
#include "CtrlPanel.h"
#include "IComponent.h"
#include "Crt.h"
#include "MessageCycle.h"

CtrlPanel::CtrlPanel(Crt &crtRef, HyEntity2d *pParent /*= nullptr*/) :
	HyUiContainer(HYORIEN_Vertical, HyPanelInit(), pParent),
	m_CrtRef(crtRef),
	m_btnVolume_Down(HyPanelInit(120, 50, 2), HyNodePath("", "CtrlPanel"), this),
	m_btnVolume_Up(HyPanelInit(120, 50, 2), HyNodePath("", "CtrlPanel"), this)
{
	m_btnVolume_Down.SetText("VOL -");
	m_btnVolume_Down.SetButtonClickedCallback([this](HyButton *pThis, void *pData)
	{
		HyEngine::Audio().SetGlobalVolume(HyMath::Clamp(HyEngine::Audio().GetGlobalVolume() - 0.05f, 0.0f, 1.0f));
		m_CrtRef.SetVolume(HyEngine::Audio().GetGlobalVolume());
	});

	m_btnVolume_Up.SetText("VOL +");
	m_btnVolume_Up.SetButtonClickedCallback([this](HyButton *pThis, void *pData)
	{
		HyEngine::Audio().SetGlobalVolume(HyMath::Clamp(HyEngine::Audio().GetGlobalVolume() + 0.05f, 0.0f, 1.0f));
		m_CrtRef.SetVolume(HyEngine::Audio().GetGlobalVolume());
	});

	SetSize(HyEngine::Window(1).GetWidth(), HyEngine::Window(1).GetHeight());

	HyLayoutHandle hCurRow = InsertLayout(HYORIEN_Horizontal);
	InsertWidget(m_btnVolume_Down, hCurRow);
	InsertWidget(m_btnVolume_Up, hCurRow);
}

/*virtual*/ CtrlPanel::~CtrlPanel()
{
}