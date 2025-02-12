#include "pch.h"
#include "ConsolePage.h"
#include "Compositorium.h"
#include "GameBrowser.h"

ConsolePage::ConsolePage(HyEntity2d *pParent /*= nullptr*/) :
	HyUiContainer(HYORIENT_Horizontal, HyPanelInit(GAMEBROWSER_WIDTH, GAMEBROWSER_HEIGHT/*, 0, HyColor(0.0f, 1.0f, 0.0f, 0.3f)*/), pParent),
	m_iHoverIndex(-1)
{
	m_RootLayout.SetMargins(GAMEBROWSER_MARGINS, 5);

	std::vector<GameConsole> consoleList = Compositorium::Get()->GetConsoleList();

	for(int iConsoleIndex = 0; iConsoleIndex < consoleList.size(); ++iConsoleIndex)
	{
		HyButton *pNewBtn = HY_NEW HyButton(HyPanelInit(GAMEBROWSER_WIDTH / 3, GAMEBROWSER_HEIGHT / 9/*, 0, HyColor(1.0f, 0.0f, 0.0f, 0.3f)*/));

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
	InsertSpacer(HYSIZEPOLICY_Fixed, 125);
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
		m_LogoSpriteList[i]->pos.Set(GAMEBROWSER_WIDTH * 0.5f, GAMEBROWSER_HEIGHT * 0.25f);

		glm::vec3 vScale(1.0f);
		glm::quat quatRot;
		glm::vec3 ptTranslation;
		glm::vec3 vSkew;
		glm::vec4 vPerspective;
		glm::decompose(m_LogoBtnList[i]->GetSceneTransform(0.0f), vScale, quatRot, ptTranslation, vSkew, vPerspective);

		ptTranslation += glm::vec3(GAMEBROWSER_WIDTH * 0.5f, GAMEBROWSER_HEIGHT * 0.5f, 0.0f);
		ptTranslation += glm::vec3(m_LogoBtnList[i]->GetWidth(0.5f), m_LogoBtnList[i]->GetHeight(0.5f), 0.0f);

		glm::vec2 ptDest(ptTranslation.x, ptTranslation.y);
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
