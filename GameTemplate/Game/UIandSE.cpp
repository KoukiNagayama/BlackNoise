#include "stdafx.h"
#include "UIandSE.h"
#include "GameCamera.h"
#include "Bell.h"
#include "sound/SoundEngine.h"

namespace
{
	const float DISTANCE = 100.0f;		//プレイヤーとの距離
}

UIandSE::~UIandSE()
{

}

bool UIandSE::Start()
{
	//画像の読み込み
	m_spriteRender.Init("Assets/sprite/Abutton.dds", 128, 128);
	m_spriteRender.SetPosition(Vector3::Zero);

	//ゲームカメラ
	m_gamecam = FindGO<GameCamera>("gamecamera");
	//ベル
	m_bell = FindGO<Bell>("bell");
	return true;
}

void UIandSE::DrawSprite()
{
	if (m_bell->IsRing() != false) {
		m_drawSprite = true;
	}
}

void UIandSE::Render(RenderContext& rc)
{
	if (m_drawSprite != false)
	{
		m_spriteRender.Draw(rc);
	}
}