#include "stdafx.h"
#include "Record.h"
#include "GameCamera.h"
#include "Item.h"

namespace
{
	const float DISTANCE = 75.0f;		// プレイヤーがレコードを取得する距離
}
bool Record::Start()
{
	m_modelRender.Init("Assets/modelData/item/record_item.tkm");

	m_gameCamera = FindGO<GameCamera>("gamecamera");

	m_item = FindGO<Item>("item");

	if (m_number == 1) {
		m_position = Vector3(300.0f, 100.0f, 0.0f);
	}
	else if (m_number == 2) {
		m_position = Vector3(0.0f, 50.0f, 0.0f);
	}

	m_modelRender.SetTRS(m_position, m_rotation, m_scale);
	
	return true;
}

void Record::Update()
{

	// プレイヤーとの距離
	// カメラの座標
	m_gameCameraPos = m_gameCamera->GetPosition();
	m_gameCameraPos.y = m_position.y;
	// 座標の差
	Vector3	distToPlayer = m_position - m_gameCameraPos;
	// 一定距離近づいたら
	if (distToPlayer.Length() <= DISTANCE) {
		// レコードを取得したとして保留アイテムに設定する
		m_item->SetRecordToPendingItem(m_number);
		DeleteGO(this);
	}
	m_modelRender.Update();
}

void Record::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
