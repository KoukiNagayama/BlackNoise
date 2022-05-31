#include "stdafx.h"
#include "GameClear.h"
#include "GroundFloor.h"
#include "Title.h"

namespace
{
	const float   GAME_CLEAR_SPRITE_W = 1600.0f;						// ゲームクリア時のスプライトの横幅
	const float   GAME_CLEAR_SPRITE_H = 900.0f;						// ゲームクリア時のスプライトの縦幅
	const Vector3 GAME_CLEAR_SPRITE_POSITION = Vector3::Zero;		// ゲームクリア時のスプライトの座標
	const Vector3 GAME_CLEAR_SPRITE_SCALE = Vector3::One;			// ゲームクリア時のスプライトの拡大率
	const float	  TIMER_TO_DRAW_SPRITE = 5.0f;
}
GameClear::~GameClear()
{

}

bool GameClear::Start()
{
	m_gameClearSprite.Init("Assets/sprite/GameClear.DDS", GAME_CLEAR_SPRITE_W, GAME_CLEAR_SPRITE_H);

	m_gameClearSprite.SetPosition(GAME_CLEAR_SPRITE_POSITION);
	m_gameClearSprite.SetScale(GAME_CLEAR_SPRITE_SCALE);

	m_timerToDrawSprite = TIMER_TO_DRAW_SPRITE;
	m_groundFloor = FindGO<GroundFloor>("groundfloor");

	m_isGameClear = true;
	return true;
}

void GameClear::Update()
{
	m_timerToDrawSprite -= 1.0f * g_gameTime->GetFrameDeltaTime();

	if (m_timerToDrawSprite <= 0.0f && m_isDraw == false) {
		m_isDraw = true;
		DeleteGO(m_groundFloor);
		g_infoForEdge.Clear();
	}

	if (m_isDraw == true) {
		if (g_pad[0]->IsTrigger(enButtonA)) {

			m_title = NewGO<Title>(0, "title");
			DeleteGO(this);
		}
	}
	

	m_gameClearSprite.Update();
}

void GameClear::Render(RenderContext& rc)
{
	if (m_isDraw == true) {
		m_gameClearSprite.Draw(rc);
	}
}
