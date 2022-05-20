#include "stdafx.h"
#include "UI.h"
#include "sound/SoundEngine.h"

bool UI::Start()
{
	//m_spriteRender.Init("Assets/sprite/Abutton.dds", 128, 128);
	m_spriteRender.SetPosition(Vector3(0.0f, -130.0f, 0.0f));
	//”j‰ó‰¹‚Ì“Ç‚Ýž‚Ý
	g_soundEngine->ResistWaveFileBank(10, "Assets/sound/se/item_get.wav");
	return true;
}

void UI::Update()
{

}

void UI::MakeGetSound()
{
	m_se = NewGO<SoundSource>(10);
	m_se->Init(10);
	m_se->SetVolume(1.0f);
	m_se->Play(false);
}

void  UI::Render(RenderContext& rc)
{
	if (m_isDraw != false)
	{
		m_spriteRender.Draw(rc);
	}
}