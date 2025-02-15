#include "pch.h"
#include "ConsolePage.h"
#include "Compositorium.h"
#include "GameBrowser.h"
#include "Melody.h"

#define CONSOLEPAGE_SPINE_WIDTH 275
#define CONSOLEPAGE_WIDGET_SPACING 15

ConsolePage::ConsolePage(HyEntity2d *pParent /*= nullptr*/) :
	HyUiContainer(HYORIENT_Horizontal, HyPanelInit(GAMEBROWSER_WIDTH, GAMEBROWSER_HEIGHT), pParent),
	m_iHoverIndex(-1)
{
	m_RootLayout.SetMargins(GAMEBROWSER_MARGINS, CONSOLEPAGE_WIDGET_SPACING);
	SetDefaultWidgetSpacing(CONSOLEPAGE_WIDGET_SPACING, true);

	std::vector<GameConsole> consoleList = Compositorium::Get()->GetConsoleList();

	for(int iConsoleIndex = 0; iConsoleIndex < consoleList.size(); ++iConsoleIndex)
	{
		int iNumPerColumn = (consoleList.size() / 2) + 1;
		int iBtnHeight = (GAMEBROWSER_LAYOUT_HEIGHT - (iNumPerColumn * CONSOLEPAGE_WIDGET_SPACING)) / iNumPerColumn;

		HyButton *pNewBtn = HY_NEW HyButton(HyPanelInit((GAMEBROWSER_LAYOUT_WIDTH - CONSOLEPAGE_SPINE_WIDTH) / 3, iBtnHeight));

		HySprite2d *pNewSprite = HY_NEW HySprite2d(HyNodePath("Consoles/Logo"), this);
		pNewSprite->SetState(iConsoleIndex);
		m_LogoSpriteList.push_back(pNewSprite);

		GameConsole eConsole = consoleList[iConsoleIndex];
		pNewBtn->SetButtonClickedCallback(
			[this, eConsole](HyButton *pThis)
			{
				if(IsInputAllowed() == false)
					return;

				static_cast<GameBrowser *>(ParentGet())->BrowseAtGame(Compositorium::Get()->GetAlphaJumpGame(eConsole, "A"));
			});

		m_LogoBtnList.push_back(pNewBtn);
	}

	InsertSpacer(HYSIZEPOLICY_Expanding);
	HyLayoutHandle hCol1 = InsertLayout(HYORIENT_Vertical);// , hMain);
	InsertSpacer(HYSIZEPOLICY_Fixed, CONSOLEPAGE_SPINE_WIDTH);
	HyLayoutHandle hCol2 = InsertLayout(HYORIENT_Vertical);// , hMain);
	InsertSpacer(HYSIZEPOLICY_Expanding);

	int iColumnRows = consoleList.size() / 2;
	for(int iConsoleIndex = 0; iConsoleIndex < consoleList.size(); ++iConsoleIndex)
	{
		HyLayoutHandle hColumn = iConsoleIndex < iColumnRows ? hCol1 : hCol2;
		InsertWidget(*m_LogoBtnList[iConsoleIndex], hColumn);
	}
}

/*virtual*/ ConsolePage::~ConsolePage()
{
	for(int i = 0; i < m_LogoSpriteList.size(); ++i)
		delete m_LogoSpriteList[i];

	for(int i = 0; i < m_LogoBtnList.size(); ++i)
		delete m_LogoBtnList[i];
}

/*virtual*/ void ConsolePage::OnContainerUpdate() /*override*/
{
}

/*virtual*/ float ConsolePage::OnBeginShow() /*override*/
{
	for(int i = 0; i < m_LogoSpriteList.size(); ++i)
	{
		m_LogoSpriteList[i]->scale.SetAll(1.0f);
		glm::vec2 vMaxSize(m_LogoBtnList[i]->GetWidth(), m_LogoBtnList[i]->GetHeight());
		float fScale = std::min(vMaxSize.x / m_LogoSpriteList[i]->GetWidth(), vMaxSize.y / m_LogoSpriteList[i]->GetHeight());
		m_LogoSpriteList[i]->scale.SetAll(fScale);

		m_LogoSpriteList[i]->pos.Set(GAMEBROWSER_WIDTH * 0.5f, GAMEBROWSER_HEIGHT * 0.25f);
		glm::vec2 ptDest = GetWidgetPos(*m_LogoBtnList[i]);
		ptDest += vMaxSize * 0.5f;
		m_LogoSpriteList[i]->pos.BezierQuick(ptDest, true, 0.75f, 1.0f, HyTween::QuadInOut, i * 0.1f);

		m_LogoSpriteList[i]->alpha.Set(0.0f);
		m_LogoSpriteList[i]->alpha.Tween(1.0f, 1.0f, HyTween::Linear, i * 0.1f);
	}

	return 0.0f;
}

/*virtual*/ void ConsolePage::OnShown() /*override*/
{ 
	SetVisible(true);
	SetInputAllowed(true);
}

/*virtual*/ float ConsolePage::OnBeginHide() /*override*/
{
	return 0.0f;
}

/*virtual*/ void ConsolePage::OnHidden() /*override*/
{
	SetVisible(false);
	SetInputAllowed(false);
}
