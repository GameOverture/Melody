#ifndef Docket_h__
#define Docket_h__

#include "pch.h"
#include "IComponent.h"

class Docket : public IComponent
{
	HyButton				m_CtrlPanel_AddGameBtn;
	HyButton				m_CtrlPanel_SaveBtn;

	enum ReloadState
	{
		RELOADSTATE_Idle,
		RELOADSTATE_FadingOut,
		RELOADSTATE_Reload,
	};
	ReloadState						m_eReloadState;
	std::string						m_sHtmlFilePath;

	
	std::vector<GameStats>			m_GameList;

public:
	Docket(HyEntity2d *pParent = nullptr);
	virtual ~Docket();

	virtual void PopulateCtrlPanel(CtrlPanel &ctrlPanel) override;
	virtual void Show(float fDuration) override;
	virtual void Hide(float fDuration) override;

	void LoadFile();
	void SaveFile();

protected:
	virtual void OnUpdate() override;
};

#endif // Docket_h__
