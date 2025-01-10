#ifndef NowPlaying_h__
#define NowPlaying_h__

#include "pch.h"
#include "IComponent.h"

class NowPlaying : public IComponent
{
	HyCheckBox				m_CtrlPanel_CheckBox;
	HyButton				m_CtrlPanel_BrowseBtn;

	HyText2d				m_Title;
	HyTexturedQuad2d		m_BoxArt;
	HyRichLabel				m_Description;

public:
	NowPlaying(HyEntity2d *pParent = nullptr);
	virtual ~NowPlaying();

	virtual void PopulateCtrlPanel(CtrlPanel &ctrlPanel) override;

protected:
	virtual void OnUpdate() override;
};

#endif // NowPlaying_h__
