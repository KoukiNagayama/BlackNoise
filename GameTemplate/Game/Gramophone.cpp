#include "stdafx.h"
#include "Gramophone.h"
#include "InfoForEdge.h"
#include "GameCamera.h"
#include "sound/SoundEngine.h"

namespace
{
	const float RANGE = 1000.0f;					// 影響範囲
	const float EDGE_FADE_IN_DELTA_VALUE = 0.07f;	// エッジがフェードインするときの変位量
	const float EDGE_FADE_OUT_DELTA_VALUE = 0.01f;	// エッジがフェードアウトするときの変位量
	const float RATE_BY_TIME_MAX_VALUE = 1.00f;		// 時間による影響率の最大値
	const float RATE_BY_TIME_MIN_VALUE = 0.00f;		// 時間による影響率の最小値
	const float MAXIMUM_VOLUME = 2.00f;				// 最大音量
	const float MINIMUM_VOLUME = 0.00f;				// 最小音量
}
bool Gramophone::Start()
{
	m_modelRender.Init("Assets/modelData/item/toy.tkm");
	m_position = Vector3::Zero;
	m_scale = Vector3::One;
	m_modelRender.SetTRS(m_position, m_rotation, m_scale);

	//サウンドを登録。
	g_soundEngine->ResistWaveFileBank(2, "Assets/sound/record/record5.wav");
	g_infoForEdge.InitForSound(2, m_position, RANGE, 0, m_rateByTime1);

	m_gameCamera = FindGO<GameCamera>("gamecamera");
	return true;
}

void Gramophone::Update()
{
	m_gameCamera = FindGO<GameCamera>("gamecamera");


	ManegeState();

	Font();
	m_modelRender.Update();
}

void Gramophone::ManegeState()
{
	switch (m_gramophoneState)
	{
	// 待機中ならば
	case enGramophoneState_Idle:
		IdleState();
		break;
	// 演奏中ならば
	case enGramophoneState_Play:
		PlayState();
		break;
	default:
		break;
	}
}


void Gramophone::IdleState()
{
	if (m_gramophoneState == enGramophoneState_Idle) {
		m_gramophoneState = enGramophoneState_Play;
	}
}

void Gramophone::PlayState()
{
	if (m_soundSource == nullptr) {
		MakeSound();
	}

	VolumeControl();
}

void Gramophone::MakeSound()
{
	m_soundSource = NewGO<SoundSource>(2);
	m_soundSource->Init(2);
	m_soundSource->Play(true);
	g_infoForEdge.SetIsSound(2, 1);
}

float Gramophone::SoundLevelByDistance()
{
	Vector3 gameCameraPos = m_gameCamera->GetPosition();
	gameCameraPos.y = m_position.y;

	//プレイヤーと蓄音機の距離
	Vector3 diff = m_position - gameCameraPos;
	
	// 音の大きさを計算
	float soundLevel = MAXIMUM_VOLUME - (diff.Length() / RANGE * MAXIMUM_VOLUME);
	// 最小音量より小さくなったら固定する
	if (soundLevel <= MINIMUM_VOLUME) {
		soundLevel = MINIMUM_VOLUME;
	}

	return soundLevel;
}

void Gramophone::VolumeControl()
{
	m_soundSource->SetVolume(SoundLevelByDistance());
}

void Gramophone::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
	m_font.Draw(rc);
}

void Gramophone::Font()
{
	wchar_t wcsbuf[256];
	swprintf_s(wcsbuf, 256, L"soundLevel:%.3f", SoundLevelByDistance());
	//表示するテキストを設定。
	m_font.SetText(wcsbuf);
	//フォントの位置を設定。
	m_font.SetPosition(Vector3(360.0f, 400.0f, 0.0f));
	//フォントの大きさを設定。
	m_font.SetScale(1.0f);
}
