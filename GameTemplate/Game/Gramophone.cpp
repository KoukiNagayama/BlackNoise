#include "stdafx.h"
#include "Gramophone.h"
#include "InfoForEdge.h"
#include "GameCamera.h"
#include "sound/SoundEngine.h"
#include "Item.h"

namespace
{
	const float DISTANCE = 130.0f;					//蓄音機までの距離
	const float RANGE = 1000.0f;						// 影響範囲
	const float EDGE_FADE_IN_DELTA_VALUE = 0.07f;	// エッジがフェードインするときの変位量
	const float EDGE_FADE_OUT_DELTA_VALUE = 0.01f;	// エッジがフェードアウトするときの変位量
	const float RATE_BY_TIME_MAX_VALUE = 1.00f;		// 時間による影響率の最大値
	const float RATE_BY_TIME_MIN_VALUE = 0.00f;		// 時間による影響率の最小値
	const float MAXIMUM_VOLUME = 0.2f;				// 最大音量
	const float MINIMUM_VOLUME = 0.00f;				// 最小音量
}
Gramophone::~Gramophone()
{
	for (int i = 1; i <= 3; i++)
	{
		g_infoForEdge.SetIsSound(i, 0);
		g_infoForEdge.SetRate(i, 0.00f);
	}
	DeleteGO(m_soundSource1);
	DeleteGO(m_soundSource2);
	DeleteGO(m_soundSource3);

}
bool Gramophone::Start()
{
	// 初期化
	Init(m_number);

	// ゲームカメラ
	m_gameCamera = FindGO<GameCamera>("gamecamera");

	m_item = FindGO<Item>("item");

	return true;
}

void Gramophone::Init(int num)
{
	if (num == 1) {
		// 初期は待機状態
		m_gramophoneState = enGramophoneState_Idle;
		m_modelRender.Init("Assets/modelData/item/record_off.tkm", nullptr, 0, false, enModelUpAxisZ, false, 0, 1);
		// サウンドを登録
		g_soundEngine->ResistWaveFileBank(2, "Assets/sound/record/record3.wav");
		g_infoForEdge.InitForSound(2, m_position, RANGE, 0, m_rateByTime);
	}

	else if (num == 2) {
		// 初期は待機状態
		m_gramophoneState = enGramophoneState_Idle;
		m_modelRender.Init("Assets/modelData/item/record_off.tkm", nullptr, 0, false, enModelUpAxisZ, false, 0, 1);
		// サウンドを登録
		g_soundEngine->ResistWaveFileBank(3, "Assets/sound/record/record4.wav");
		g_infoForEdge.InitForSound(3, m_position, RANGE, 0, m_rateByTime);
	}
	
	else if (num == 3) {
		// 初期から演奏中
		m_gramophoneState = enGramophoneState_Play;
		m_modelRender.Init("Assets/modelData/item/record_on.tkm", nullptr, 0, false, enModelUpAxisZ, false, 0, 1);
		// サウンドを登録
		g_soundEngine->ResistWaveFileBank(1, "Assets/sound/record/record7.wav");
		g_infoForEdge.InitForSound(4, m_position, RANGE, 0, m_rateByTime);
	}

	// モデルの座標、回転、拡大を設定
	m_modelRender.SetTRS(m_position, m_rotation, m_scale);

}

void Gramophone::Update()
{
	m_gameCamera = FindGO<GameCamera>("gamecamera");

	m_item = FindGO<Item>("item");

	// ステート管理
	ManegeState();

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

	Vector3 gameCameraPos = m_gameCamera->GetPosition();
	gameCameraPos.y = m_position.y;

	//プレイヤーと蓄音機の距離
	Vector3 diff = m_position - gameCameraPos;
	
	// 範囲内でAボタンを押したら
	if (diff.Length() <= DISTANCE && g_pad[0]->IsTrigger(enButtonA)) {
		m_recordNum = m_item->GetHaveRecord();
		if(m_recordNum == 1 || m_recordNum == 2){
			m_item->IsUseRecord(m_recordNum);
			m_gramophoneState = enGramophoneState_Play;
			m_modelRender.Init("Assets/modelData/item/record_on.tkm");
		}
	}
}

void Gramophone::PlayState()
{
	if (m_number == 1 && m_soundSource1 == nullptr
		|| m_number == 2 && m_soundSource2 == nullptr
		|| m_number == 3 && m_soundSource3 == nullptr) 
	{
		MakeSound();
	}


	VolumeControl();

	ChangeRate(m_number);
}

void Gramophone::MakeSound()
{
	if (m_number == 1) {
		m_soundSource1 = NewGO<SoundSource>(2);
		if (m_recordNum == 1) {
			m_soundSource1->Init(2);
		}
		else if (m_recordNum == 2) {
			m_soundSource1->Init(3);
		}
		m_soundSource1->Play(false);
		g_infoForEdge.SetIsSound(2, 1);
	}
	else if(m_number == 2){
		m_soundSource2 = NewGO<SoundSource>(3);
		if (m_recordNum == 1) {
			m_soundSource2->Init(2);
		}
		else if (m_recordNum == 2) {
			m_soundSource2->Init(3);
		}
		m_soundSource2->Play(false);
		g_infoForEdge.SetIsSound(3, 1);
	}
	else if (m_number == 3) {
		m_soundSource3 = NewGO<SoundSource>(1);
		m_soundSource3->Init(1);
		m_soundSource3->Play(true);
		g_infoForEdge.SetIsSound(4, 1);
	}

}

float Gramophone::SoundLevelByDistance(float range)
{
	Vector3 gameCameraPos = m_gameCamera->GetPosition();
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

void Gramophone::VolumeControl()
{
	if (m_number == 1) {
		m_soundSource1->SetVolume(SoundLevelByDistance(RANGE));
	}
	else if (m_number == 2) {
		m_soundSource2->SetVolume(SoundLevelByDistance(RANGE));
	}
	else if (m_number == 3) {
		m_soundSource3->SetVolume(SoundLevelByDistance(RANGE));
	}
}

void Gramophone::ChangeRate(int num)
{
	int check;

	// 番号が1なら
	if (num == 1) {
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
			g_infoForEdge.SetIsSound(2, check);
			g_infoForEdge.SetRate(2, m_rateByTime);
		}
	}
	// 番号が2なら
	else if (num == 2) {
		if (m_soundSource2 != nullptr) {
			// 音源が再生中
			if (m_soundSource2->IsPlaying() == true)
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
					if (m_rateByTime <= RATE_BY_TIME_MIN_VALUE) {
						m_rateByTime = RATE_BY_TIME_MIN_VALUE;
					}
				}
			}
			g_infoForEdge.SetIsSound(3, check);
			g_infoForEdge.SetRate(3, m_rateByTime);
		}
	}
	// 番号が3なら
	else if (num == 3) {
		if (m_soundSource3 != nullptr) {
			if (m_soundSource3->IsPlaying() == true)
			{
				check = 1;
				if (m_rateByTime < RATE_BY_TIME_MAX_VALUE) {
					m_rateByTime += EDGE_FADE_IN_DELTA_VALUE;
				}
			}
			else {
				check = 0;
				if (m_rateByTime > RATE_BY_TIME_MIN_VALUE && check == 0) {
					m_rateByTime -= EDGE_FADE_OUT_DELTA_VALUE;
					if (m_rateByTime <= RATE_BY_TIME_MIN_VALUE) {
						m_rateByTime = RATE_BY_TIME_MIN_VALUE;
					}
				}
			}
			g_infoForEdge.SetIsSound(4, check);
			g_infoForEdge.SetRate(4, m_rateByTime);
		}
	}
}

void Gramophone::Render(RenderContext& rc)
{
	//m_modelRender.Draw(rc);
	//m_font.Draw(rc);
}


