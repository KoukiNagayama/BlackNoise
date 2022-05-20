#include "stdafx.h"
#include "TitleSprite.h"
#include "Title.h"

namespace
{
	const Vector3 SPRITE_POSITION1 = Vector3(-7.7f, -10.0f, 0.0f);
	const Vector3 SPRITE_POSITION2 = Vector3(7.0f, -20.8f, 0.0f);
	const float SPRITE_SCALE = 0.9f;
}
bool TitleSprite::Start()
{
	m_spriteRender1.Init("Assets/sprite/PRESS_A_TO_PLAY_MUSIC.dds", 1920, 1080);
	m_spriteRender2.Init("Assets/sprite/PRESS_A_TO_START.dds", 1920, 1080);

	m_spritePos1 = SPRITE_POSITION1;
	m_spritePos2 = SPRITE_POSITION2;

	m_spriteScale = Vector3(SPRITE_SCALE, SPRITE_SCALE, SPRITE_SCALE);

	// ç¿ïWÅAägëÂó¶Çê›íË
	m_spriteRender1.SetPosition(m_spritePos1);
	m_spriteRender1.SetScale(m_spriteScale);
	m_spriteRender2.SetPosition(m_spritePos2);
	m_spriteRender2.SetScale(m_spriteScale);


	m_title = FindGO<Title>("title");

	return true;
}

void TitleSprite::Update()
{
	m_titleState = m_title->GetTitleState();

	m_spriteRender1.Update();
	m_spriteRender2.Update();
}

void TitleSprite::Render(RenderContext& rc)
{
	if (m_titleState == 0) {
		m_spriteRender1.Draw(rc);
	}
	else {
		m_spriteRender2.Draw(rc);
	}
}
