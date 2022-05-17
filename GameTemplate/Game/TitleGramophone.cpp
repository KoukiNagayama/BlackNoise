#include "stdafx.h"
#include "TitleGramophone.h"
#include "Title.h"
#include "InfoForEdge.h"
#include "sound/SoundEngine.h"
#include "TitleCamera.h"

namespace
{
	const float RANGE = 700.0f;						// 影響範囲
	const float EDGE_FADE_IN_DELTA_VALUE = 0.01f;	// エッジがフェードインするときの変位量
	const float EDGE_FADE_OUT_DELTA_VALUE = 0.01f;	// エッジがフェードアウトするときの変位量
	const float RATE_BY_TIME_MAX_VALUE = 1.00f;		// 時間による影響率の最大値
	const float RATE_BY_TIME_MIN_VALUE = 0.00f;		// 時間による影響率の最小値
	const float MAXIMUM_VOLUME = 2.00f;				// 最大音量
	const float MINIMUM_VOLUME = 0.00f;				// 最小音量
}
TitleGramophone::~TitleGramophone()
{

}
bool TitleGramophone::Start()
{
	// 初期化
	Init();

	// ゲームカメラ
	m_titleCamera = FindGO<TitleCamera>("titleCamera");

	return true;
}

void TitleGramophone::Init()
{
	// 初期は待機状態
	m_gramophoneState = enGramophoneState_Idle;
	m_modelRender.Init("Assets/modelData/item/record_on.tkm", nullptr, 0, false, enModelUpAxisZ, false, 0, 1);
	// サウンドを登録
	g_soundEngine->ResistWaveFileBank(14, "Assets/sound/record/record5.wav");
	g_infoForEdge.InitForSound(0, m_position, RANGE, 0, m_rateByTime);

	// モデルの座標、回転、拡大を設定
	m_modelRender.SetTRS(m_position, m_rotation, m_scale);

	// タイトル
	m_title = FindGO<Title>("title");

}

void TitleGramophone::Update()
{
	m_titleCamera = FindGO<TitleCamera>("titleCamera");

	// ステート管理
	ManegeState();

	m_modelRender.Update();
}

void TitleGramophone::ManegeState()
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


void TitleGramophone::IdleState()
{
	// タイトルのステートが待機中ならば
	if (m_title->GetTitleState() != 2) {
		return;
	}
	// タイトル用蓄音機のステートを演奏中に変更する
	m_gramophoneState = enGramophoneState_Play;
}

void TitleGramophone::PlayState()
{
	if (m_soundSource1 == nullptr) {
		// 音を鳴らす
		MakeSound();
	}

	// 音量調整
	VolumeControl();

	// 影響率
	ChangeRate();
}

void TitleGramophone::MakeSound()
{

	m_soundSource1 = NewGO<SoundSource>(0);
	m_soundSource1->Init(14);
	m_soundSource1->Play(true);
	g_infoForEdge.SetIsSound(0, 1);
}

float TitleGramophone::SoundLevelByDistance(float range)
{
	Vector3 gameCameraPos = m_titleCamera->GetPosition();
	gameCameraPos.y = m_position.y;

	//プレイヤーと蓄音機の距離
	Vector3 diff = m_position - gameCameraPos;

	// 音の大きさ
	float soundLevel = MAXIMUM_VOLUME - (diff.Length() / range * MAXIMUM_VOLUME);
	// 最小音量より小さくなったら固定する
	if (soundLevel <= MINIMUM_VOLUME) {
		soundLevel = MINIMUM_VOLUME;
	}

	return soundLevel;
}

void TitleGramophone::VolumeControl()
{
	m_soundSource1->SetVolume(SoundLevelByDistance(RANGE));
}

void TitleGramophone::ChangeRate()
{
	int check;
	if (m_soundSource1 != nullptr) {
		// 音源が再生中
		if (m_soundSource1->IsPlaying() == true)
		{
			check = 1;
			// 影響率を増やす
			if (m_rateByTime < RATE_BY_TIME_MAX_VALUE) {
				m_rateByTime += EDGE_FADE_IN_DELTA_VALUE;
			}
		}
		// 音源が再生されていない時
		else {
			check = 0;
			// 影響率を減らす
			if (m_rateByTime > RATE_BY_TIME_MIN_VALUE && check == 0) {
				m_rateByTime -= EDGE_FADE_OUT_DELTA_VALUE;
				// 
				if (m_rateByTime <= RATE_BY_TIME_MIN_VALUE) {
					m_rateByTime = RATE_BY_TIME_MIN_VALUE;
				}
			}
		}
		g_infoForEdge.SetIsSound(0, check);
		g_infoForEdge.SetRate(0, m_rateByTime);
	}
	
}

void TitleGramophone::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}


