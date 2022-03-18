#include "stdafx.h"
#include "GameStart.h"
#include "Player.h"
#include "GameCamera.h"
#include "BackGround.h"
#include "Bell.h"
#include "InfoForEdge.h"


GameStart::GameStart()
{
}

GameStart::~GameStart()
{

}

bool GameStart::Start()
{
	Vector3 position = Vector3::Zero;
	g_infoForEdge.InitForSound(0, position, 200.0f, 1);
	//各クラスを生成。
	m_gamecam = NewGO<GameCamera>(0, "gamecamera");
	m_player = NewGO<Player>(0, "player");
	m_bg = NewGO<BackGround> (0, "background");
	m_bell = NewGO<Bell>(0, "bell");
	return true;
}

void GameStart::Update()
{
	g_infoForEdge.SetIsSound(1, true);
}

void GameStart::Render(RenderContext& rc)
{
}
