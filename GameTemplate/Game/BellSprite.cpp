#include "stdafx.h"
#include "BellSprite.h"
#include "Bell.h"

namespace
{
	const Vector3 BELL_POSITION = Vector3::Zero;					// ベルの座標
	const Vector3 BELL_SCALE = Vector3(0.3f, 0.3f, 0.3f);			// ベルの拡大率
}

bool BellSprite::Start()
{
	// スプライトの初期化
	m_bellSprite.Init("Assets/sprite/Bell.DDS", 500, 862);

	// スプライトの座標
	m_bellSpritePos = BELL_POSITION;
	// スプライトの拡大率
	m_bellSpriteSca = BELL_SCALE;

	m_bellSprite.SetPosition(m_bellSpritePos);
	m_bellSprite.SetScale(m_bellSpriteSca);

	m_bell = FindGO<Bell>("bell");
	return true;
}

void BellSprite::Update()
{
	// α値を増やす
	if (m_currentAlpha <= 1.0f) {
		m_currentAlpha += 1.0f * g_gameTime->GetFrameDeltaTime();
	}
	// ベルが鳴ったならば
	if (m_bell->IsNeverRung() == false) {
		DeleteGO(this);
	}
	m_bellSprite.Update();
}

void BellSprite::Render(RenderContext& rc)
{
	if (m_bell->IsNeverRung() == true) {
		m_bellSprite.SetMulColor({ 1.0f, 1.0f, 1.0f, m_currentAlpha });
		m_bellSprite.Draw(rc);
	}
}
