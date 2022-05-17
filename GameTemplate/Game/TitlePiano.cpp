#include "stdafx.h"
#include "TitlePiano.h"
#include "sound/SoundEngine.h"
#include "Title.h"

namespace
{
	const float RANGE = 100.0f;						// �e���͈�
	const float EDGE_FADE_IN_DELTA_VALUE = 0.01f;	// �G�b�W���t�F�[�h�C������Ƃ��̕ψʗ�
	const float EDGE_FADE_OUT_DELTA_VALUE = 0.01f;	// �G�b�W���t�F�[�h�A�E�g����Ƃ��̕ψʗ�
	const float RATE_BY_TIME_MAX_VALUE = 1.00f;		// ���Ԃɂ��e�����̍ő�l
	const float RATE_BY_TIME_MIN_VALUE = 0.00f;		// ���Ԃɂ��e�����̍ŏ��l
	const float VOLUME = 0.5f;
}
bool TitlePiano::Start()
{
	// �T�E���h��o�^
	g_soundEngine->ResistWaveFileBank(15, "Assets/sound/piano/do.wav");
	g_infoForEdge.InitForSound(1, m_position, RANGE, 0, m_rateByTime);
	

	return true;
}

void TitlePiano::Update()
{
	if (m_soundSource == nullptr && m_count <= 4) {
		MakeSound();
	}
		
	// �e����
	ChangeRate();

	if (m_soundSource == nullptr && m_count == 4) {
		// �~���@��炷
		m_title->StartPlayGramophone();
	}
}

void TitlePiano::MakeSound()
{
	// �o�^���ꂽ�������Đ�
	m_soundSource = NewGO<SoundSource>(0);
	m_soundSource->Init(15);
	m_soundSource->Play(false);
	m_soundSource->SetVolume(VOLUME);
	g_infoForEdge.SetIsSound(0, 1);
	// �s�A�m�������񐔂𑝂₷
	m_count++;
}

void TitlePiano::ChangeRate()
{
	int check;
	if (m_soundSource != nullptr) {
		// �������Đ���
		if (m_soundSource->IsPlaying() == true)
		{
			check = 1;
			// �e�����𑝂₷
			if (m_rateByTime < RATE_BY_TIME_MAX_VALUE) {
				m_rateByTime += EDGE_FADE_IN_DELTA_VALUE;
			}
		}
		// �������Đ�����Ă��Ȃ���
		else {
			check = 0;
			// �e���������炷
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

