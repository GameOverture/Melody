#include "pch.h"
#include "InputViewer.h"
#include "CtrlPanel.h"
#include "StreetFighter.h"
#include "NES.h"
#include "StarCitizen.h"

#define RETRO_POS_X		600.0f
#define RETRO_POS_Y		75.0f
#define RETRO_HIDEPOS_Y	-300.0f

InputViewer::InputViewer(HyEntity2d *pParent /*= nullptr*/) :
	IComponent(COMPONENT_InputViewer, pParent),
	m_CtrlPanel_CheckBox(HyPanelInit(32, 32, 2), HyNodePath("", "CtrlPanel")),
	m_eActiveController(INPUTCONTROLLER_StreetFighter),
	m_bShowRetro(false)
{
	m_CtrlPanel_CheckBox.SetText("Input Viewer");
	m_CtrlPanel_CheckBox.SetCheckedChangedCallback(
		[this](HyCheckBox *pCheckBox)
		{
			if(pCheckBox->IsChecked())
				Show(0.5f);
			else
				Hide(0.5f);
		});

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
			[this](HyRadioButton *pRadio)
			{
				if(pRadio->IsChecked())
				{
					m_Controllers[m_eActiveController]->SetVisible(false);
					m_eActiveController = static_cast<InputController>(pRadio->GetTag());
					m_Controllers[m_eActiveController]->SetVisible(true);

					if(m_eActiveController == INPUTCONTROLLER_NES)
					{
						static_cast<NESController *>(m_Controllers[INPUTCONTROLLER_NES])->Connect();
						if(m_bShowRetro)
							RetroIntro();
					}
					else if(m_bShowRetro)
						RetroOutro();

				}
			});
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

	m_Controllers[INPUTCONTROLLER_NES]->pos.SetY(RETRO_HIDEPOS_Y);
}

/*virtual*/ InputViewer::~InputViewer()
{
	for(int i = 0; i < NUM_INPUTCONTROLLERS; ++i)
		delete m_Controllers[i];
}

/*virtual*/ void InputViewer::PopulateCtrlPanel(CtrlPanel &ctrlPanel) /*override*/
{
	HyLayoutHandle hRow = ctrlPanel.InsertLayout(HYORIENT_Horizontal);
	ctrlPanel.InsertWidget(m_CtrlPanel_CheckBox, hRow);
	ctrlPanel.InsertSpacer(HYSIZEPOLICY_Expanding, 0, hRow);

	hRow = ctrlPanel.InsertLayout(HYORIENT_Horizontal);
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

void InputViewer::SetShowRetro(bool bShow)
{
	m_bShowRetro = bShow;

	if(m_eActiveController == INPUTCONTROLLER_NES)
	{
		if(m_bShowRetro)
			RetroIntro();
		else
			RetroOutro();
	}
}

void InputViewer::RetroIntro()
{
	m_Controllers[INPUTCONTROLLER_NES]->pos.Set(RETRO_POS_X, RETRO_HIDEPOS_Y);
	m_Controllers[INPUTCONTROLLER_NES]->pos.Tween(RETRO_POS_X, RETRO_POS_Y + 32.0f, 0.5f, HyTween::QuadInOut, 0.0f, [](IHyNode *pThis) { static_cast<HyEntity2d *>(pThis)->pos.Tween(RETRO_POS_X, RETRO_POS_Y, 0.5f, HyTween::QuadInOut); });

	m_Controllers[INPUTCONTROLLER_NES]->rot.Set(HyRand::Boolean() ? HyRand::Range(10.0f, 20.0f) : HyRand::Range(-10.0f, -20.0f));
	m_Controllers[INPUTCONTROLLER_NES]->rot.Tween(m_Controllers[INPUTCONTROLLER_NES]->rot.Get() > 0.0f ? -4.0f : 4.0f, 0.5f, HyTween::BounceOut, 0.0f, [](IHyNode *pThis) { static_cast<HyEntity2d *>(pThis)->rot.Tween(-4.0f, 0.5f, HyTween::QuadInOut); });

	m_Controllers[INPUTCONTROLLER_NES]->scale.Set(1.0f, 1.0f);
	m_Controllers[INPUTCONTROLLER_NES]->scale.Tween(0.75f, 0.75f, 1.0f, HyTween::QuadInOut);
}

void InputViewer::RetroOutro()
{
	m_Controllers[INPUTCONTROLLER_NES]->pos.Tween(RETRO_POS_X, RETRO_HIDEPOS_Y, 1.0f, HyTween::QuadIn);
	m_Controllers[INPUTCONTROLLER_NES]->rot.Tween(HyRand::Boolean() ? HyRand::Range(10.0f, 20.0f) : HyRand::Range(-10.0f, -20.0f), 1.0f, HyTween::QuadIn);
}

/*virtual*/ void InputViewer::OnUpdate() /*override*/
{
	if(IsVisible())
		m_Controllers[m_eActiveController]->ApplyInputs();
}
