#include "stdafx.h"
#include "Toy.h"
#include "GameCamera.h"

Toy::Toy()
{
	m_modelRender.Init("Assets/modelData/item/key.tkm");
	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();
}

Toy::~Toy()
{

}

void Toy::Update()
{
	// ステートが通常なら
	if (m_toyState == enToyState_usual) {
		// プレイヤーとの距離
		// カメラの座標
		m_gameCameraPos = m_gameCamera->GetPosition();
		m_gameCameraPos.y = 0.0f;
		// 座標の差
		Vector3	distToPlayer = m_position - m_gameCameraPos;
		if (distToPlayer.Length() <= 10.0f) {
			// モデルを変更
			m_modelRender.Init("Assets/modelData/item/clash_toy.tkm");
			// ステートを変更
			m_toyState = enToyState_collapse;
		}
	}

	m_modelRender.Update();
}
void Toy::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}