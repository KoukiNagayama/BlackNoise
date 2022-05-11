#include "stdafx.h"
#include "Hammer.h"
#include "BrokenDoor.h"
#include "GameCamera.h"

namespace
{
	const float DISTANCE = 100.0f;	//プレイヤーまでの距離
}

bool Hammer::Start()
{
	//モデルの読み込み
	m_modelRender.Init("Assets/modelData/item/hammer.tkm", nullptr, 0, false, enModelUpAxisZ, false, 0, 1);
	//座標の設定
	m_modelRender.SetPosition(m_position);
	//モデルの更新
	m_modelRender.Update();

	//ブロークンドア
	m_b_door = FindGO<BrokenDoor>("brokendoor");
	//ゲームカメラ
	m_gamecam = FindGO<GameCamera>("gamecamera");

	return true;
}

void Hammer::Update()
{
	Vector3 cameraPos = m_gamecam->GetPosition();
	cameraPos.y = m_position.y;
	m_disToPlayer = m_position - cameraPos;
	//拾える範囲だったら
	if (m_disToPlayer.Length() <= DISTANCE)
	{
		m_b_door->PickHammer();
		DeleteGO(this);
	}
	m_modelRender.Update();
}

void Hammer::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}