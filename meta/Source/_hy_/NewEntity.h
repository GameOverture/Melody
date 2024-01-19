/********************************************************************************
** Auto generated class by Harmony Engine - Editor Tool
**
** WARNING! All changes made in this file will be lost when resaving the entity!
********************************************************************************/
#ifndef NewEntity_h__
#define NewEntity_h__

#include "pch.h"

namespace hy {

class NewEntity : public HyEntity2d
{
protected:
	const float				m_fFRAME_DURATION;
	std::function<void()>	m_fpUpdateFunc;
	float					m_fElapsedFrameTime;
	uint32					m_uiCurFrame;

	HyTexturedQuad2d		m_frame_01_delay-0;
	HyTexturedQuad2d		m_LK_Released;
	HyTexturedQuad2d		m_HK_Pressed;

public:
	NewEntity(HyEntity2d *pParent = nullptr);
	virtual ~NewEntity();
	
	virtual bool SetState(uint32 uiStateIndex) override;
	virtual uint32 GetNumStates() override;
	
protected:
	virtual void Update() override;
};

} // 'hy' namespace

#endif // NewEntity_h__
