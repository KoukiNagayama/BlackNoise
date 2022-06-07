#include "stdafx.h"
#include "TitleSprite.h"
#include "Title.h"

namespace
{
	const Vector3 BELL_POSITION = Vector3::Zero;						// ベルの座標
	const Vector3 TEXT_POSITION = Vector3(7.0f, -20.8f, 0.0f);			// テキストの座標
	const Vector3 B_POSITION = Vector3(0.0f, 30.0f, 0.0f);				// Bの座標
	const Vector3 PRESS_POSITION = Vector3(0.0f, -180.0f, 0.0f);		// PRESSの座標
	const Vector3 BELL_SCALE = Vector3(0.3f, 0.3f, 0.3f);				// ベルの拡大率
	const Vector3 TEXT_SCALE = Vector3(0.9f, 0.9f, 0.9f);				// テキストの拡大率
	const Vector3 B_SCALE = Vector3(0.35f, 0.35f, 0.35f);				// Bの拡大率
	const Vector3 PRESS_SCALE = Vector3::One;							// PRESSの拡大率
	const float   FADE_OUT_ALPHA_MULTIPLIER = 0.3f;						// フェードアウトのαの倍率
	const float   BELL_SPRITE_W = 500.0f;								// ベルのスプライトの横幅
	const float	  BELL_SPRITE_H = 862.0f;								// ベルのスプライトの縦幅
	const float   TEXT_SPRITE_W = 1920.0f;								// テキストのスプライトの横幅
	const float   TEXT_SPRITE_H = 1080.0f;								// テキストのスプライトの縦幅
	const float   B_SPRITE_W = 512.0f;									// Bのスプライトの横幅
	const float   B_SPRITE_H = 512.0f;									// Bのスプライトの縦幅
	const float   PRESS_SPRITE_W = 512.0f;								// PRESSのスプライトの横幅
	const float   PRESS_SPRITE_H = 512.0f;								// PRESSのスプライトの縦幅
}
bool TitleSprite::Start()
{
	// 各スプライトを初期化
	m_bellSprite.Init("Assets/sprite/Bell.DDS", BELL_SPRITE_W, BELL_SPRITE_H);
	m_textSprite.Init("Assets/sprite/PRESS_A_TO_START.dds", TEXT_SPRITE_W, TEXT_SPRITE_H);
	m_bButtonSprite.Init("Assets/sprite/B.DDS", B_SPRITE_W, B_SPRITE_H);
	m_pressSprite.Init("Assets/sprite/PRESS.DDS", PRESS_SPRITE_W, PRESS_SPRITE_H);

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
		m_currentAlpha -= FADE_OUT_ALPHA_MULTIPLIER * g_gameTime->GetFrameDeltaTime();
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
