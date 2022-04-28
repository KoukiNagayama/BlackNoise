#include "stdafx.h"
#include "Key.h"
#include "GameCamera.h"
#include "GroundFloor.h"
namespace
{
	const float DISTANCE = 100.0f;
}

bool Key::Start()
{
	//モデルの読み込み
	m_modelRender.Init("Assets/modelData/item/hammer.tkm");
	//座標の設定
	m_modelRender.SetTRS(m_position, m_rotation, m_scale);
	//モデルの更新
	m_modelRender.Update();

	//ゲームカメラ
	m_gamecam = FindGO<GameCamera>("gamecamera");
	//一階
	m_groundfloor = FindGO<GroundFloor>("groundfloor");

	return true;
}

void Key::Update()
{
	if (m_havePiece < 2)
	{
		return;
	}
	Vector3 cameraPos = m_gamecam->GetPosition();
	cameraPos.y = m_position.y;
	Vector3 disToPlayer = m_position - cameraPos;
	//拾える範囲だったら
	if (disToPlayer.Length() <= DISTANCE)
	{
		m_groundfloor->PickKey();
		DeleteGO(this);
	}
	m_modelRender.Update();
}

void Key::Render(RenderContext& rc)
{
	if (m_havePiece < 2)
	{
		return;
	}
	m_modelRender.Draw(rc);
}