#include "stdafx.h"
#include "TitlePiano.h"
#include "sound/SoundEngine.h"
#include "Title.h"

namespace
{
	const float RANGE = 100.0f;						// 影響範囲
	const float EDGE_FADE_IN_DELTA_VALUE = 0.01f;	// エッジがフェードインするときの変位量
	const float EDGE_FADE_OUT_DELTA_VALUE = 0.01f;	// エッジがフェードアウトするときの変位量
	const float RATE_BY_TIME_MAX_VALUE = 1.00f;		// 時間による影響率の最大値
	const float RATE_BY_TIME_MIN_VALUE = 0.00f;		// 時間による影響率の最小値
	const float VOLUME = 0.5f;
}
bool TitlePiano::Start()
{
	// サウンドを登録
	g_soundEngine->ResistWaveFileBank(15, "Assets/sound/piano/do.wav");
	g_infoForEdge.InitForSound(1, m_position, RANGE, 0, m_rateByTime);
	

	return true;
}

void TitlePiano::Update()
{
	if (m_soundSource == nullptr && m_count <= 4) {
		MakeSound();
	}
		
	// 影響率
	ChangeRate();

	if (m_soundSource == nullptr && m_count == 4) {
		// 蓄音機を鳴らす
		m_title->StartPlayGramophone();
	}
}

void TitlePiano::MakeSound()
{
	// 登録された音源を再生
	m_soundSource = NewGO<SoundSource>(0);
	m_soundSource->Init(15);
	m_soundSource->Play(false);
	m_soundSource->SetVolume(VOLUME);
	g_infoForEdge.SetIsSound(0, 1);
	// ピアノが鳴った回数を増やす
	m_count++;
}

void TitlePiano::ChangeRate()
{
	int check;
	if (m_soundSource != nullptr) {
		// 音源が再生中
		if (m_soundSource->IsPlaying() == true)
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
		g_infoForEdge.SetIsSound(1, check);
		g_infoForEdge.SetRate(1, m_rateByTime);
	}
}

