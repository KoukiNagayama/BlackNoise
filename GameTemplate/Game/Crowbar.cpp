#include "stdafx.h"
#include "Crowbar.h"
#include "GameCamera.h"
#include "Item.h"

namespace
{
	const float DISTANCE = 100.0f;	//プレイヤーまでの距離
}

bool Crowbar::Start()
{
	//モデルの読み込み
	m_modelRender.Init("Assets/modelData/item/crowbar.tkm");
	//座標の設定
	m_modelRender.SetTRS(m_position, m_rotation, m_scale);
	//モデルの更新
	m_modelRender.Update();

	//ブロークンドア
	m_item = FindGO<Item>("item");
	//ゲームカメラ
	m_gamecam = FindGO<GameCamera>("gamecamera");

	return true;
}

void Crowbar::Update()
{
	Vector3 cameraPos = m_gamecam->GetPosition();
	cameraPos.y = m_position.y;
	Vector3 disToPlayer = m_position - cameraPos;
	//拾える範囲だったら
	if (disToPlayer.Length() <= DISTANCE)
	{
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			m_item->SetCrowbarToPendingItem();
			DeleteGO(this);
		}
	}
	m_modelRender.Update();
}

void Crowbar::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
