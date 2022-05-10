#include "stdafx.h"
#include "UI.h"
#include "sound/SoundEngine.h"

bool UI::Start()
{
	m_spriteRender.Init("Assets/sprite/Abutton.dds", 931, 465);
	m_spriteRender.SetPosition(Vector3(0.0f, -130.0f, 0.0f));
	//”j‰ó‰¹‚Ì“Ç‚Ýž‚Ý
	g_soundEngine->ResistWaveFileBank(10, "Assets/sound/se/item_get.wav");
	return true;
}

void UI::Update()
{

}
