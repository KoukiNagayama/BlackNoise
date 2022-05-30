#include "stdafx.h"
#include "TitleSprite.h"
#include "Title.h"

namespace
{
	const Vector3 BELL_POSITION = Vector3::Zero;
	const Vector3 TEXT_POSITION = Vector3(7.0f, -20.8f, 0.0f);
	const Vector3 B_POSITION = Vector3(0.0f, 30.0f, 0.0f);
	const Vector3 PRESS_POSITION = Vector3(0.0f, -180.0f, 0.0f);
	const Vector3 BELL_SCALE = Vector3(0.3f, 0.3f, 0.3f);
	const Vector3 TEXT_SCALE = Vector3(0.9f, 0.9f, 0.9f);
	const Vector3 B_SCALE = Vector3(0.35f, 0.35f, 0.35f);
	const Vector3 PRESS_SCALE = Vector3::One;
}
bool TitleSprite::Start()
{
	// 各スプライトを初期化
	m_bellSprite.Init("Assets/sprite/Bell.DDS", 500, 862);
	m_textSprite.Init("Assets/sprite/PRESS_A_TO_START.dds", 1920, 1080);
	m_bButtonSprite.Init("Assets/sprite/B.DDS", 512, 512);
	m_pressSprite.Init("Assets/sprite/PRESS.DDS", 512, 512);

	// スプライトの座標を指定
	m_bellPos = BELL_POSITION;
	m_textPos = TEXT_POSITION;
	m_bButtonPos = B_POSITION;
	m_pressPos = PRESS_POSITION;

	// スプライトの拡大率を指定
	m_bellScale = BELL_SCALE;
	m_textScale = TEXT_SCALE;
	m_bButtonScale = B_SCALE;
	m_pressScale = PRESS_SCALE;

	// 指定した座標、拡大率を設定
	m_bellSprite.SetPosition(m_bellPos);
	m_bellSprite.SetScale(m_bellScale);
	m_textSprite.SetPosition(m_textPos);
	m_textSprite.SetScale(m_textScale);
	m_bButtonSprite.SetPosition(m_bButtonPos);
	m_bButtonSprite.SetScale(m_bButtonScale);
	m_pressSprite.SetPosition(m_pressPos);
	m_pressSprite.SetScale(m_pressScale);

	// タイトルオブジェクトを検索
	m_title = FindGO<Title>("title");

	return true;
}

void TitleSprite::Update()
{
	// フェードアウト中はスプライトをフェードアウトさせる
	if (m_title->IsFadeOutStateNow()) {
		m_currentAlpha -= 0.3f * g_gameTime->GetFrameDeltaTime();
		// 現在のα値が0以下ならば
		if (m_currentAlpha <= 0.0f) {
			m_currentAlpha = 0.0f;
		}
	}
	// 各スプライトを更新
	m_bellSprite.Update();
	m_textSprite.Update();
	m_bButtonSprite.Update();
	m_pressSprite.Update();
}

void TitleSprite::Render(RenderContext& rc)
{
	// ステートが待機中ならば
	if (m_title->IsIdleStateNow() == true) {
		m_bellSprite.Draw(rc);
		m_bButtonSprite.Draw(rc);
		m_pressSprite.Draw(rc);
	}
	// ステートが待機中以外ならば
	else{
		if (m_currentAlpha > 0.0f) {
			m_textSprite.SetMulColor({ 1.0f,1.0f,1.0f,m_currentAlpha });
			m_textSprite.Draw(rc);
		}
	}
}
