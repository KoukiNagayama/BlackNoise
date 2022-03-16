#include "stdafx.h"
#include "GameStart.h"
#include "Player.h"
#include "GameCamera.h"
#include "BackGround.h"

GameStart::GameStart()
{
}

GameStart::~GameStart()
{

}

bool GameStart::Start()
{
	//各クラスを生成。
	m_gamecam = NewGO<GameCamera>(0, "gamecamera");
	m_player = NewGO<Player>(0, "player");
	m_bg = NewGO<BackGround> (0, "background");
	return true;
}

void GameStart::Update()
{
}

void GameStart::Render(RenderContext& rc)
{
}
