#include "stdafx.h"
#include "Record.h"
#include "GameCamera.h"
#include "Item.h"
#include "UI.h"

namespace
{
	const float DISTANCE = 160.0f;		// プレイヤーがレコードを取得する距離
}
bool Record::Start()
{
	m_modelRender.Init("Assets/modelData/item/record_item.tkm", nullptr, 0, false, enModelUpAxisZ, false, 0, 1);

	m_gameCamera = FindGO<GameCamera>("gamecamera");

	m_item = FindGO<Item>("item");

	m_ui = FindGO<UI>("ui");

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
	if (distToPlayer.Length() <= DISTANCE && g_pad[0]->IsTrigger(enButtonA)) {
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
