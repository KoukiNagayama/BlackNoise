#include "stdafx.h"
#include "Toy.h"
#include "GameCamera.h"
#include "sound/SoundEngine.h"

//<<<<<<< HEAD
namespace
{
	const float LENGTH = 50.0f;
}

Toy::Toy()
{
}

Toy::~Toy()
{
	
=======
namespace {
	const float DISTANCE = 20.0f;
	const float EDGE_FADE_IN_DELTA_VALUE = 0.05f;	// エッジがフェードインするときの変位量
	const float EDGE_FADE_OUT_DELTA_VALUE = 0.05f;	// エッジがフェードアウトするときの変位量
	const float RATE_BY_TIME_MAX_VALUE = 1.00f;		// 時間による影響率の最大値
	const float RATE_BY_TIME_MIN_VALUE = 0.00f;		// 時間による影響率の最小値
	const float SOUND_RANGE = 400.0f;
>>>>>>> a62433f5daeb7ac427daa0af05158f5cbb09f2e8
}

bool Toy::Start()
{
	m_modelRender.Init("Assets/modelData/item/toy.tkm");
	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();
	m_gameCamera = FindGO<GameCamera>("gamecamera");

	g_soundEngine->ResistWaveFileBank(3, "Assets/sound/toy/toy.wav");
	g_infoForEdge.InitForSound(3, m_position, SOUND_RANGE, 0, m_rateByTime);
	g_infoForEdge.SetRate(3, m_rateByTime);
	return true;
}

void Toy::Update()
{
<<<<<<< HEAD
	// プレイヤーとの距離
	// カメラの座標
	m_gameCameraPos = m_gameCamera->GetPosition();
	m_gameCameraPos.y = 0.0f;
	// 座標の差
	Vector3	distToPlayer = m_position - m_gameCameraPos;
	if (distToPlayer.Length() <= LENGTH) {
		if (m_toyState == enToyState_usual) {
=======
	// ステートが通常なら
	if (m_toyState == enToyState_usual) {
		// プレイヤーとの距離
		// カメラの座標
		m_gameCameraPos = m_gameCamera->GetPosition();
		m_gameCameraPos.y = 0.0f;
		// 座標の差
		Vector3	distToPlayer = m_position - m_gameCameraPos;
		if (distToPlayer.Length() <= DISTANCE) {
			// モデルを変更
>>>>>>> a62433f5daeb7ac427daa0af05158f5cbb09f2e8
			m_modelRender.Init("Assets/modelData/item/clash_toy.tkm");
			// ステートを変更
			m_toyState = enToyState_collapse;

			MakeSound();
		}
	}

	CheckRate();

	m_modelRender.Update();
}

void Toy::MakeSound()
{
	m_sound = NewGO<SoundSource>(3);
	m_sound->Init(3);
	m_sound->SetVolume(1.0f);
	m_sound->Play(false);
}

void Toy::CheckRate()
{
	int check1;
	if (m_sound != nullptr) {
		if (m_sound->IsPlaying() == true)
		{
			check1 = 1;
			if (m_rateByTime < RATE_BY_TIME_MAX_VALUE) {
				m_rateByTime += EDGE_FADE_IN_DELTA_VALUE;
			}
		}
		else {
			check1 = 0;
			if (m_rateByTime > RATE_BY_TIME_MIN_VALUE && check1 == 0) {
				m_rateByTime -= EDGE_FADE_OUT_DELTA_VALUE;
				if (m_rateByTime <= RATE_BY_TIME_MIN_VALUE) {
					m_rateByTime = RATE_BY_TIME_MIN_VALUE;
				}
			}
		}
		g_infoForEdge.SetIsSound(3, check1);
		g_infoForEdge.SetRate(3, m_rateByTime);
	}
}
void Toy::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}