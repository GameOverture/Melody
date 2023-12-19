#ifndef VgMusic_h__
#define VgMusic_h__

#include "pch.h"

class VgMusic : public HyEntity2d
{
	HyTexturedQuad2d			m_BoxArt;
	HyTexturedQuad2d			m_Title;
	HyTexturedQuad2d			m_Snapshot;

	HyText2d					m_AudioTrackText;

	std::vector<std::string>	m_MusicFileList;
	int32						m_iCurrTrackIndex;

public:
	VgMusic(HyEntity2d *pParent = nullptr);
	virtual ~VgMusic();

protected:
	virtual void OnUpdate() override;

	void NextTrack();
};

#endif // VgMusic_h__
