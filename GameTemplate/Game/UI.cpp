#include "stdafx.h"
#include "UI.h"
#include "sound/SoundEngine.h"

bool UI::Start()
{
	m_spriteRender.Init("Assets/sprite/Abutton.dds", 128, 128);
	m_spriteRender.SetPosition(Vector3(0.0f, -130.0f, 0.0f));
	//破壊音の読み込み
	g_soundEngine->ResistWaveFileBank(10, "Assets/sound/se/item_get.wav");
	return true;
}

void UI::Update()
{

}
