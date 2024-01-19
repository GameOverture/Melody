/********************************************************************************
** Auto generated class by Harmony Engine - Editor Tool
**
** WARNING! All changes made in this file will be lost when resaving the entity!
********************************************************************************/
#include "pch.h"
#include "NewEntity.h"

namespace hy {

NewEntity::NewEntity(HyEntity2d *pParent /*= nullptr*/) :
	HyEntity2d(pParent),
	m_fFRAME_DURATION(0.016667f),
	m_fpUpdateFunc(nullptr),
	m_fElapsedFrameTime(0.0f),
	m_uiCurFrame(0),
	m_frame_01_delay-0(2292571689, 0, this),
	m_LK_Released(3547625482, 0, this),
	m_HK_Pressed(330467946, 0, this)
{
	SetState(0);
}

/*virtual*/ NewEntity::~NewEntity()
{
}

/*virtual*/ bool NewEntity::SetState(uint32 uiStateIndex)
{
	if(HyEntity2d::SetState(uiStateIndex) == false || uiStateIndex >= GetNumStates())
		return false;
	
	switch(m_uiState)
	{
	case 0:
		m_fpUpdateFunc = [this]()
		{
			std::vector<glm::vec2> vertList;
			m_fElapsedFrameTime += HyEngine::DeltaTime();
			while(m_fElapsedFrameTime >= 0.0f)
			{
				switch(m_uiCurFrame)
				{
				default:
					break;

				case 0:
					m_frame_01_delay-0.pos.Set(-330.000031f, -133.380035f);
					m_frame_01_delay-0.scale.Set(1.375000f, 0.988000f);
					m_HK_Pressed.pos.Set(-33.500000f, -34.000000f);
					m_LK_Released.pos.Set(33.520020f, 34.020020f);
					break;

				}
				m_uiCurFrame++;
				m_fElapsedFrameTime -= m_fFRAME_DURATION;
			}
		};
	}
	
	m_fElapsedFrameTime = 0.0f;
	m_uiCurFrame = 0;
}

/*virtual*/ uint32 NewEntity::GetNumStates()
{
	return 1;
}

/*virtual*/ void NewEntity::Update() /*override*/
{
	m_fpUpdateFunc();
	HyEntity2d::Update();
}

} // 'hy' namespace
