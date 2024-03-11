#include "pch.h"
#include "InputViewer.h"
#include "CtrlPanel.h"
#include "StreetFighter.h"
#include "NES.h"
#include "StarCitizen.h"

InputViewer::InputViewer(HyEntity2d *pParent /*= nullptr*/) :
	IComponent(COMPONENT_InputViewer, pParent),
	m_CtrlPanel_CheckBox(HyPanelInit(32, 32, 2), HyNodePath("", "CtrlPanel")),
	m_eActiveController(INPUTCONTROLLER_StreetFighter)
{
	m_CtrlPanel_CheckBox.SetText("Input Viewer");
	m_CtrlPanel_CheckBox.SetCheckedChangedCallback(
		[this](HyCheckBox *pCheckBox, void *pData)
		{
			if(pCheckBox->IsChecked())
				reinterpret_cast<IComponent *>(pData)->Show(0.5f);
			else
				reinterpret_cast<IComponent *>(pData)->Hide(0.5f);
		}, this);

	for(int i = 0; i < NUM_INPUTCONTROLLERS; ++i)
	{
		m_CtrlPanel_radController[i].Setup(HyPanelInit(24, 24, 2), HyNodePath("", "CtrlPanel"));
		m_CtrlPanel_radController[i].SetTag(i);
		m_BtnGrp.AddButton(m_CtrlPanel_radController[i]);
	}
	m_CtrlPanel_radController[INPUTCONTROLLER_StreetFighter].SetChecked(true);
	m_CtrlPanel_radController[INPUTCONTROLLER_StarCitizen].SetText("SC");
	m_CtrlPanel_radController[INPUTCONTROLLER_NES].SetText("NES");
	m_CtrlPanel_radController[INPUTCONTROLLER_StreetFighter].SetText("SF");

	for(int i = 0; i < NUM_INPUTCONTROLLERS; ++i)
	{
		m_CtrlPanel_radController[i].SetCheckedChangedCallback(
			[this](HyRadioButton *pRadio, void *pData)
			{
				if(pRadio->IsChecked())
				{
					InputViewer *pThis = reinterpret_cast<InputViewer *>(pData);
					pThis->m_Controllers[pThis->m_eActiveController]->SetVisible(false);
					pThis->m_eActiveController = static_cast<InputController>(pRadio->GetTag());
					pThis->m_Controllers[pThis->m_eActiveController]->SetVisible(true);

					if(pThis->m_eActiveController == INPUTCONTROLLER_NES)
						static_cast<NESController *>(pThis->m_Controllers[INPUTCONTROLLER_NES])->Connect();
				}
			}, this);
	}

	// Assemble the controllers dynamically
	for(int i = 0; i < NUM_INPUTCONTROLLERS; ++i)
	{
		switch(i)
		{
		case INPUTCONTROLLER_StreetFighter:
			m_Controllers[i] = HY_NEW StreetFighter(this);
			break;

		case INPUTCONTROLLER_NES:
			m_Controllers[i] = HY_NEW NESController(this);
			break;

		case INPUTCONTROLLER_StarCitizen:
			m_Controllers[i] = HY_NEW StarCitizen(this);
			break;
		}
		
		if(i != m_eActiveController)
			m_Controllers[i]->SetVisible(false);
	}
}

/*virtual*/ InputViewer::~InputViewer()
{
	for(int i = 0; i < NUM_INPUTCONTROLLERS; ++i)
		delete m_Controllers[i];
}

/*virtual*/ void InputViewer::PopulateCtrlPanel(CtrlPanel &ctrlPanel) /*override*/
{
	HyLayoutHandle hRow = ctrlPanel.InsertLayout(HYORIEN_Horizontal);
	ctrlPanel.InsertWidget(m_CtrlPanel_CheckBox, hRow);
	ctrlPanel.InsertSpacer(HYSIZEPOLICY_Expanding, 0, hRow);

	hRow = ctrlPanel.InsertLayout(HYORIEN_Horizontal);
	for(int i = 0; i < NUM_INPUTCONTROLLERS; ++i)
		ctrlPanel.InsertWidget(m_CtrlPanel_radController[i], hRow);
	ctrlPanel.InsertSpacer(HYSIZEPOLICY_Expanding, 0, hRow);
}

/*virtual*/ void InputViewer::Show(float fDuration) /*override*/
{
	IComponent::Show(fDuration);
}

/*virtual*/ void InputViewer::Hide(float fDuration) /*override*/
{
	IComponent::Hide(fDuration);
}

/*virtual*/ void InputViewer::OnUpdate() /*override*/
{
	if(IsVisible())
		m_Controllers[m_eActiveController]->ApplyInputs();
}
