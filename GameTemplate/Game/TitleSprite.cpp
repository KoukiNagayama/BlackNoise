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
	// 各スプライトを初期化
	m_spriteRender1.Init("Assets/sprite/PRESS_A_TO_PLAY_MUSIC.dds", 1920, 1080);
	m_spriteRender2.Init("Assets/sprite/PRESS_A_TO_START.dds", 1920, 1080);

	// スプライトの座標を指定
	m_spritePos1 = SPRITE_POSITION1;
	m_spritePos2 = SPRITE_POSITION2;

	// スプライトの拡大率を指定
	m_spriteScale = Vector3(SPRITE_SCALE, SPRITE_SCALE, SPRITE_SCALE);

	// 指定した座標、拡大率を設定
	m_spriteRender1.SetPosition(m_spritePos1);
	m_spriteRender1.SetScale(m_spriteScale);
	m_spriteRender2.SetPosition(m_spritePos2);
	m_spriteRender2.SetScale(m_spriteScale);

	// タイトルオブジェクトを検索
	m_title = FindGO<Title>("title");

	return true;
}

void TitleSprite::Update()
{
	// 各スプライトを更新
	m_spriteRender1.Update();
	m_spriteRender2.Update();
}

void TitleSprite::Render(RenderContext& rc)
{
	// ステートが待機中ならば
	if (m_title->IsIdleStateNow() == true) {
		m_spriteRender1.Draw(rc);
	}
	// ステートが待機中以外ならば
	else {
		m_spriteRender2.Draw(rc);
	}
}
