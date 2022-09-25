#include "pch.h"
#include "Music.h"

Dancer::Dancer(HyEntity2d *pParent /*= nullptr*/) :
	HyEntityLeaf2d("GameSprites", "Dancer", pParent)
{
	m_Leaf.scale.Set(2.0f, 2.0f);
}

void Dancer::Dance()
{
	m_bIsShimmy = false;
	m_Leaf.SetAnimCallback(m_Leaf.GetState(), nullptr);
	m_Leaf.SetAnimCtrl(HYANIMCTRL_Play);
	m_Leaf.SetAnimCtrl(HYANIMCTRL_DontBounce);
}

void Dancer::DanceAlt()
{
	m_bIsShimmy = false;
	m_Leaf.SetAnimCallback(m_Leaf.GetState(), nullptr);
	m_Leaf.SetAnimCtrl(HYANIMCTRL_Play);
	m_Leaf.SetAnimCtrl(HYANIMCTRL_Bounce);
}

void Dancer::Stop()
{
	m_bIsShimmy = false;
	m_Leaf.SetAnimCallback(m_Leaf.GetState(), 
		[](HySprite2d *pSelf, void *pParam)
		{
			pSelf->SetAnimCtrl(HYANIMCTRL_Reset);
			pSelf->SetAnimPause(true);
		}, this);
}

void Dancer::Shimmy()
{
	m_Leaf.SetAnimCallback(m_Leaf.GetState(),
		[](HySprite2d *pSelf, void *pParam)
		{
			pSelf->SetAnimCtrl(HYANIMCTRL_Reset);
			pSelf->SetAnimPause(true);
		}, this);

	m_fElapsedTime = 0.0f;
	m_bIsShimmy = true;
}

/*virtual*/ void Dancer::OnUpdate() /*override*/
{
	if(m_bIsShimmy)
	{
		if(m_Leaf.GetFrame() == 0 || m_Leaf.GetFrame() == 1)
		{
			m_fElapsedTime += HyEngine::DeltaTime();

			if(m_fElapsedTime > 0.5f)
			{
				m_Leaf.SetFrame(m_Leaf.GetFrame() == 0 ? 1 : 0);
				m_fElapsedTime = 0.0f;
			}
		}
	}
}

Music::Music(HyEntity2d *pParent /*= nullptr*/) :
	Channel(pParent),
	m_pBoxArt(nullptr)
{
	for(int32 i = 0; i < NUM_DANCERS; ++i)
	{
		ChildAppend(m_Dancers[i]);
		m_Dancers[i].GetLeaf().SetState(i);
	}

	m_Dancers[0].pos.Set(100.0f, 50.0f);
	m_Dancers[1].pos.Set(220.0f, 120.0f);
	m_Dancers[2].pos.Set(340.0f, 50.0f);
	m_Dancers[3].pos.Set(460.0f, 120.0f);
	m_Dancers[4].pos.Set(580.0f, 50.0f);
	m_Dancers[5].pos.Set(700.0f, 120.0f);

	m_Static.SetVisible(false);
}

/*virtual*/ Music::~Music()
{
}

/*virtual*/ void Music::OnUpdate() /*override*/
{
	if(HyEngine::Input().IsActionReleased(INPUT_Dance))
	{
		for(int32 i = 0; i < NUM_DANCERS; ++i)
			m_Dancers[i].Dance();
	}
	if(HyEngine::Input().IsActionReleased(INPUT_DanceAlt))
	{
		for(int32 i = 0; i < NUM_DANCERS; ++i)
			m_Dancers[i].DanceAlt();
	}
	if(HyEngine::Input().IsActionReleased(INPUT_DanceStop))
	{
		for(int32 i = 0; i < NUM_DANCERS; ++i)
			m_Dancers[i].Stop();
	}
	if(HyEngine::Input().IsActionReleased(INPUT_DanceShimmy))
	{
		for(int32 i = 0; i < NUM_DANCERS; ++i)
			m_Dancers[i].Shimmy();

		int32 iWidth, iHeight;
		HyTextureHandle hTex = HyEngine::HotLoadTexture("\\\\IronMountain/Documents/Video Games Meta-Scrape//NES/media/Named_Boxarts/Batman - The Video Game (USA).png", HYTEXFILTER_NEAREST, iWidth, iHeight);
		m_pBoxArt = HY_NEW HyTexturedQuad2d(hTex, iWidth, iHeight, this);
		m_pBoxArt->Load();
	}
}
