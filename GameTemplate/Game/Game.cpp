#include "stdafx.h"
#include "Game.h"
#include "Light.h";
#include "Player.h"
#include "GameCamera.h"
#include "sound/SoundEngine.h"
#include "InfoForEdge.h"
#include "Enemy.h"

namespace {
	const float EDGE_FADE_IN_DELTA_VALUE = 0.05f;	// エッジがフェードインするときの変位量
	const float EDGE_FADE_OUT_DELTA_VALUE = 0.05f;	// エッジがフェードアウトするときの変位量
	const float RATE_BY_TIME_MAX_VALUE = 1.00f;		// 時間による影響率の最大値
	const float RATE_BY_TIME_MIN_VALUE = 0.00f;		// 時間による影響率の最小値
	const float RANGE1 = 800.0f;
	const float RANGE2 = 800.0f;
}

Game::Game()
{
}

Game::~Game()
{

}

bool Game::Start()
{
	// 背景
	m_bgModelRender.Init("Assets/modelData/stage/Sample.tkm", nullptr, 0, true, enModelUpAxisZ, false);

	// 各クラスのインスタンスを生成。
	m_gamecam = NewGO<GameCamera>(0, "gamecamera");
	//m_player = NewGO<Player>(0, "player");

	// サウンドテスト
	g_soundEngine->ResistWaveFileBank(0, "Assets/sound/shot.wav");
	g_soundEngine->ResistWaveFileBank(1, "Assets/sound/shot.wav");
	//m_sound1->Play(false);
	Vector3 position;
	position.x = -100.0f;
	position.y = 0.0f;
	position.z = 0.0f;

	Vector3 position2;
	position2.x = 100.0f;
	position2.y = 0.0f;
	position2.z = 0.0f;

	g_infoForEdge.InitForSound(0, position, RANGE1, 0, rateByTime);
	g_infoForEdge.InitForSound(1, position2, RANGE2, 1, rateByTime);
	m_position.x = 100.0f;
	g_infoForEdge.SetRate(1, rateByTime);

	return true;
}

void Game::Update()
{

	m_position.x += g_pad[0]->GetLStickXF();
	m_position.z += g_pad[0]->GetLStickYF();
	Vector3 position1;
	position1.x = 0.0f;
	position1.y = 0.0f;
	position1.z = -500.0f;
	Vector3 position2;
	position2.x = 0.0f;
	position2.y = 0.0f;
	position2.z = 500.0f;
	if (g_pad[0]->IsTrigger(enButtonA)) {
		m_sound1 = NewGO<SoundSource>(0);
		m_sound1->Init(0, true);
		m_sound1->SetPosition(position1);
		m_sound1->Play(false);
	}

	if (g_pad[0]->IsTrigger(enButtonB)) {
		m_sound2 = NewGO<SoundSource>(1);
		m_sound2->Init(1, true);
		m_sound2->SetPosition(position2);
		m_sound2->Play(false);
	}
	int check1;
	if (m_sound1 != nullptr) {
		if (m_sound1->IsPlaying() == true)
		{
			check1 = 1;
			if (rateByTime2 < RATE_BY_TIME_MAX_VALUE) {
				rateByTime2 += EDGE_FADE_IN_DELTA_VALUE;
			}
		}
		else {
			check1 = 0;
			if (rateByTime2 > RATE_BY_TIME_MIN_VALUE && check1 == 0) {
				rateByTime2 -= EDGE_FADE_OUT_DELTA_VALUE;
				if (rateByTime2 <= RATE_BY_TIME_MIN_VALUE) {
					rateByTime2 = RATE_BY_TIME_MIN_VALUE;
				}
			}
		}
		g_infoForEdge.SetIsSound(0, check1);
		g_infoForEdge.SetRate(0, rateByTime2);
	}
	int check2;
	if (m_sound2 != nullptr) {
		if (m_sound2->IsPlaying() == true)
		{
			check2 = 1;
			if (rateByTime < RATE_BY_TIME_MAX_VALUE) {
				rateByTime += EDGE_FADE_IN_DELTA_VALUE;
			}
		}
		else {
			check2 = 0;
			if (rateByTime > RATE_BY_TIME_MIN_VALUE && check2 == 0) {
				rateByTime -= EDGE_FADE_OUT_DELTA_VALUE;
				if (rateByTime <= RATE_BY_TIME_MIN_VALUE) {
					rateByTime = RATE_BY_TIME_MIN_VALUE;
				}
			}
		}
		g_infoForEdge.SetIsSound(1, check2);
		g_infoForEdge.SetRate(1, rateByTime);
	}


	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_rotation);
	m_modelRender.SetScale(m_scale);
	m_bgModelRender.Update();
	m_modelRender.Update();
}

void Game::Render(RenderContext& rc)
{
	//背景を描写
	m_modelRender.Draw(rc);
	m_bgModelRender.Draw(rc);
}
