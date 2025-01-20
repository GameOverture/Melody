#ifndef NowPlaying_h__
#define NowPlaying_h__

#include "pch.h"
#include "IComponent.h"

class NowPlaying : public IComponent
{
	HyCheckBox						m_CtrlPanel_CheckBox;
	HyButton						m_CtrlPanel_BrowseBtn;

	std::vector<HyTexturedQuad2d *>	m_BoxArtList;
	int32							m_iBoxArtIndex;
	std::vector<HyTexturedQuad2d *>	m_TitleArtList;
	int32							m_iTitleArtIndex;
	
	HyTexturedQuad2d				m_Logo;
	HyText2d						m_NowPlayingText;

	HyText2d						m_Description;

	bool							m_bReloading;

public:
	NowPlaying(HyEntity2d *pParent = nullptr);
	virtual ~NowPlaying();

	virtual void PopulateCtrlPanel(CtrlPanel &ctrlPanel) override;

protected:
	virtual void OnUpdate() override;
};

#endif // NowPlaying_h__
