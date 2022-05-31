#include "stdafx.h"
#include "WhiteBoard.h"

bool WhiteBoard::Start()
{
	// モデルの初期化
	m_modelRender.Init("Assets/modelData/whiteBoard.tkm", nullptr, 0, false, enModelUpAxisZ, false, 0, 4);
	// モデルの座標、回転、拡大率を設定
	m_modelRender.SetTRS(m_position, m_rotation, m_scale);

	return true;
}

void WhiteBoard::Update()
{
	// モデルの更新
	m_modelRender.Update();
}

void WhiteBoard::Render(RenderContext& rc)
{
	// モデルの描画
	m_modelRender.Draw(rc);
}