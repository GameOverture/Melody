#include "pch.h"
#include "ConsolePage.h"
#include "Compositorium.h"
#include "GameBrowser.h"

ConsolePage::ConsolePage(HyEntity2d *pParent /*= nullptr*/) :
	HyUiContainer(HYORIENT_Vertical, HyPanelInit(GAMEBROWSER_WIDTH, GAMEBROWSER_HEIGHT, 0, HyColor(0.0f, 0.0f, 0.0f, 0.5f)), pParent),
	m_TitleLabel(HyPanelInit(GAMEBROWSER_WIDTH, 64), "MainText", this),
	m_iHoverIndex(-1)
{
	std::vector<GameConsole> consoleList = Compositorium::Get()->GetConsoleList();

	for(int iConsoleIndex = 0; iConsoleIndex < consoleList.size(); ++iConsoleIndex)
	{
		int iConsoleWidth = GAMEBROWSER_WIDTH / (consoleList.size() / 2) - 35;
		//HyLabel *pNewLabel = HY_NEW HyLabel(HyPanelInit(iConsoleWidth, 42), "Description");
		HyLabel *pNewLabel = HY_NEW HyLabel(HyPanelInit(HYTYPE_Sprite, HyNodePath("Consoles/Logo")));
		pNewLabel->SetState(iConsoleIndex);
		pNewLabel->SetAsBox();
		pNewLabel->SetTextState(2);
		m_ConsoleTitleList.push_back(pNewLabel);

		//HyButton *pNewBtn = HY_NEW HyButton(HyPanelInit(iConsoleWidth, (GAMEBROWSER_HEIGHT / 2) - 110, 0, HyColor(0.0f, 0.0f, 0.0f, 0.2f)));
		HyButton *pNewBtn = HY_NEW HyButton(HyPanelInit(HYTYPE_Sprite, HyNodePath("Consoles/Console")));
		pNewBtn->SetState(iConsoleIndex);

		GameConsole eConsole = consoleList[iConsoleIndex];
		pNewBtn->SetButtonClickedCallback(
			[this, eConsole](HyButton *pThis)
			{
				static_cast<GameBrowser *>(ParentGet())->BrowseAtGame(Compositorium::Get()->GetAlphaJumpGame(eConsole, "A"));
			});

		m_ConsoleBtnList.push_back(pNewBtn);
	}

	InsertWidget(m_TitleLabel);

	// Do rows of 5
	int iNumRows = (consoleList.size() / 5) + ((consoleList.size() % 5) > 0 ? 1 : 0);
	int iConsoleIndex = 0;
	for(int iRow = 0; iRow < iNumRows; ++iRow)
	{
		HyLayoutHandle hRow = InsertLayout(HYORIENT_Horizontal);
		for(int i = 0; i < 5 && iConsoleIndex < consoleList.size(); ++i)
		{
			HyLayoutHandle hConsole = InsertLayout(HYORIENT_Vertical, hRow);
			InsertWidget(*m_ConsoleTitleList[iConsoleIndex], hConsole);
			InsertWidget(*m_ConsoleBtnList[iConsoleIndex], hConsole);

			iConsoleIndex++;
		}
		if(iConsoleIndex >= consoleList.size())
			break;
		InsertSpacer(HYSIZEPOLICY_Fixed, 5);
	}
	InsertSpacer(HYSIZEPOLICY_Expanding);
}

/*virtual*/ ConsolePage::~ConsolePage()
{
}

/*virtual*/ void ConsolePage::OnContainerUpdate() /*override*/
{
}
