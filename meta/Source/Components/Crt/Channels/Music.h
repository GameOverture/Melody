#ifndef Music_h__
#define Music_h__

#include "pch.h"
#include "Channel.h"

#define NUM_DANCERS 6

class Dancer : public HyEntityLeaf2d<HySprite2d>
{
	bool				m_bIsShimmy;
	float				m_fElapsedTime;

public:
	Dancer(HyEntity2d *pParent = nullptr);

	void Dance();
	void DanceAlt();
	void Stop();
	void Shimmy();

protected:
	virtual void OnUpdate() override;
};

class Music : public Channel
{
	Dancer				m_Dancers[NUM_DANCERS];

	HyTexturedQuad2d *	m_pBoxArt;

public:
	Music(HyEntity2d *pParent = nullptr);
	virtual ~Music();

protected:
	virtual void OnUpdate() override;
};

#endif // Music_h__
