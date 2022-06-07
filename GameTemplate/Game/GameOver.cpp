#include "stdafx.h"
#include "GameOver.h"
#include "Enemy2.h"
#include "GameStart.h"
#include "GroundFloor.h"
#include "Title.h"
namespace
{
	const Vector3 SPRITE_POSITION = Vector3::Zero;				// スプライトの座標
	const Vector3 SPRITE_SCALE = Vector3::One;					// スプライトの拡大率
	const float   FADE_OUT_TIMER_MAX_VALUE = 1.5f;				// フェードアウトに移行するまでの最大時間
	const float	  GAME_OVER_SPRITE_W = 1920.0f;					// ゲームオーバーのスプライトの横幅
	const float	  GAME_OVER_SPRITE_H = 1080.0f;					// ゲームオーバーのスプライトの縦幅
	const float	  FADE_IN_ALPHA_MULTIPLIER = 0.5f;				// フェードインのαの倍率
	const float   FADE_OUT_ALPHA_MULTIPLIER = 0.2f;				// フェードアウトのαの倍率
}
GameOver::~GameOver()
{

}

bool GameOver::Start()
{
	// エネミーの検索
	m_enemy = FindGO<Enemy2>("enemy");

	// スプライトの初期化
	m_gameOverSprite.Init("Assets/sprite/Game_Over.DDS", GAME_OVER_SPRITE_W, GAME_OVER_SPRITE_H);
	m_gameOverPosition = SPRITE_POSITION;
	m_gameOverScale = SPRITE_SCALE;

	m_gameOverSprite.SetPosition(m_gameOverPosition);
	m_gameOverSprite.SetScale(m_gameOverScale);

	m_gameStart = FindGO<GameStart>("gameStart");
	m_groundFloor = FindGO<GroundFloor>("groundfloor");

	m_timerToFadeOut = FADE_OUT_TIMER_MAX_VALUE;

	g_infoForEdge.Clear();

	return true;
}

void GameOver::Update()
{

	// α値が1.0より低いならば
	if (m_isFadeOut == false) {
		FadeIn();
	}
	else{
		// フェードアウトするまでの時間を計算
		m_timerToFadeOut -= 1.0f * g_gameTime->GetFrameDeltaTime();
		if (m_timerToFadeOut <= 0.0f) {
			m_timerToFadeOut = 0.0f;
			FadeOut();
		}
	}
	
	m_gameOverSprite.Update();
}

void GameOver::FadeIn()
{
	// α値を増やす
	m_currentAlpha += FADE_IN_ALPHA_MULTIPLIER * g_gameTime->GetFrameDeltaTime();
	// α値が直前の式で1.0より高くなったならば
	if (m_currentAlpha >= 1.0f) {
		m_isFadeOut = true;
		DeleteObject();
	}
}

void GameOver::FadeOut()
{
	// α値を減らす
	m_currentAlpha -= FADE_OUT_ALPHA_MULTIPLIER * g_gameTime->GetFrameDeltaTime();
	// α値が直前の式で0.0より低くなったならば
	if (m_currentAlpha <= 0.0f) {
		m_title = NewGO<Title>(0, "title");
		DeleteGO(this);
	}
}

void GameOver::DeleteObject()
{
	if (m_gameStart != nullptr) {
		DeleteGO(m_gameStart);
	}
	if (m_groundFloor != nullptr) {
		DeleteGO(m_groundFloor);
	}
	g_infoForEdge.Clear();
}

void GameOver::Render(RenderContext& rc)
{
	if (m_currentAlpha >= 0.0f) {
		m_gameOverSprite.SetMulColor({ 1.0f,1.0f,1.0f,m_currentAlpha });
		m_gameOverSprite.Draw(rc);
	}
}
