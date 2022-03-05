#include "stdafx.h"
#include "Game.h"
#include "Light.h";

Game::Game()
{
}

Game::~Game()
{

}

bool Game::Start()
{
	//アニメーションのロード
	m_animationClipArray[enAnimClip_Idle].Load("Assets/animData/idle.tka");
	m_animationClipArray[enAnimClip_Idle].SetLoopFlag(true);
	m_animationClipArray[enAnimClip_Run].Load("Assets/animData/walk.tka");
	m_animationClipArray[enAnimClip_Run].SetLoopFlag(true);

	
	//モデルの初期化
	m_bgModelRender.Init("Assets/modelData/bg.tkm", nullptr, 0, true, enModelUpAxisZ, false);

	m_modelRender.Init("Assets/modelData/unityChan.tkm");

	return true;
}

void Game::Update()
{
	// 平行移動
	m_position.x += g_pad[0]->GetLStickXF();
	m_position.y += g_pad[0]->GetLStickYF();

	m_modelRender.SetPosition(m_position);

	m_modelRender.Update();

	m_bgModelRender.Update();
}

void Game::Render(RenderContext& rc)
{
	// ドロー
	m_bgModelRender.Draw(rc);

	m_modelRender.Draw(rc);
}
