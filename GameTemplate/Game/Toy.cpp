#include "stdafx.h"
#include "Toy.h"
#include "GameCamera.h"
#include "sound/SoundEngine.h"
namespace {
	const float DISTANCE = 50.0f;
	const float EDGE_FADE_IN_DELTA_VALUE = 0.05f;	// �G�b�W���t�F�[�h�C������Ƃ��̕ψʗ�
	const float EDGE_FADE_OUT_DELTA_VALUE = 0.02f;	// �G�b�W���t�F�[�h�A�E�g����Ƃ��̕ψʗ�
	const float RATE_BY_TIME_MAX_VALUE = 1.00f;		// ���Ԃɂ��e�����̍ő�l
	const float RATE_BY_TIME_MIN_VALUE = 0.00f;		// ���Ԃɂ��e�����̍ŏ��l
	const float SOUND_RANGE = 400.0f;
}

Toy::~Toy()
{
	g_infoForEdge.SetIsSound(1, 0);
	g_infoForEdge.SetRate(1, 0.00f);
	if (m_sound != nullptr) {
		DeleteGO(m_sound);
	}
}

bool Toy::Start()
{
	m_modelRender.Init("Assets/modelData/item/toy.tkm");
	m_modelRender.SetPosition(m_position);
	//m_modelRender.Update();
	m_gameCamera = FindGO<GameCamera>("gamecamera");

	g_soundEngine->ResistWaveFileBank(8, "Assets/sound/toy/toy.wav");
	g_infoForEdge.InitForSound(1, m_position, SOUND_RANGE, 0, m_rateByTime);
	return true;
}

void Toy::Update()
{
	// �X�e�[�g���ʏ�Ȃ�
	if (m_toyState == enToyState_usual) {
		// �v���C���[�Ƃ̋���
		// �J�����̍��W
		m_gameCameraPos = m_gameCamera->GetPosition();
		m_gameCameraPos.y = m_position.y;
		// ���W�̍�
		Vector3	distToPlayer = m_position - m_gameCameraPos;
		if (distToPlayer.Length() <= DISTANCE) {
			// ���f����ύX
			m_modelRender.Init("Assets/modelData/item/clash_toy.tkm");
			// �X�e�[�g��ύX
			m_toyState = enToyState_collapse;

			MakeSound();
		}
	}

	CheckRate();

	m_modelRender.Update();
}

void Toy::MakeSound()
{
	m_sound = NewGO<SoundSource>(8);
	m_sound->Init(8);
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
				m_isSound = true;
			}
		}
		else {
			check1 = 0;
			if (m_rateByTime > RATE_BY_TIME_MIN_VALUE && check1 == 0) {
				m_rateByTime -= EDGE_FADE_OUT_DELTA_VALUE;
				if (m_rateByTime <= RATE_BY_TIME_MIN_VALUE) {
					m_rateByTime = RATE_BY_TIME_MIN_VALUE;
					m_sound = nullptr;
					
				}
			}
			m_isSound = false;
		}
		g_infoForEdge.SetIsSound(1, check1);
		g_infoForEdge.SetRate(1, m_rateByTime);
	}
}
void Toy::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}