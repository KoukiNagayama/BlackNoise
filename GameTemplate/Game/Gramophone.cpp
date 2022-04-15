#include "stdafx.h"
#include "Gramophone.h"
#include "InfoForEdge.h"
#include "GameCamera.h"
#include "sound/SoundEngine.h"

namespace
{
	const float RANGE = 1000.0f;					// �e���͈�
	const float EDGE_FADE_IN_DELTA_VALUE = 0.07f;	// �G�b�W���t�F�[�h�C������Ƃ��̕ψʗ�
	const float EDGE_FADE_OUT_DELTA_VALUE = 0.01f;	// �G�b�W���t�F�[�h�A�E�g����Ƃ��̕ψʗ�
	const float RATE_BY_TIME_MAX_VALUE = 1.00f;		// ���Ԃɂ��e�����̍ő�l
	const float RATE_BY_TIME_MIN_VALUE = 0.00f;		// ���Ԃɂ��e�����̍ŏ��l
	const float MAXIMUM_VOLUME = 2.00f;				// �ő剹��
	const float MINIMUM_VOLUME = 0.00f;				// �ŏ�����
}
bool Gramophone::Start()
{
	m_modelRender.Init("Assets/modelData/item/toy.tkm");
	m_position = Vector3::Zero;
	m_scale = Vector3::One;
	m_modelRender.SetTRS(m_position, m_rotation, m_scale);

	//�T�E���h��o�^�B
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
	// �ҋ@���Ȃ��
	case enGramophoneState_Idle:
		IdleState();
		break;
	// ���t���Ȃ��
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

	//�v���C���[�ƒ~���@�̋���
	Vector3 diff = m_position - gameCameraPos;
	
	// ���̑傫�����v�Z
	float soundLevel = MAXIMUM_VOLUME - (diff.Length() / RANGE * MAXIMUM_VOLUME);
	// �ŏ����ʂ�菬�����Ȃ�����Œ肷��
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
	//�\������e�L�X�g��ݒ�B
	m_font.SetText(wcsbuf);
	//�t�H���g�̈ʒu��ݒ�B
	m_font.SetPosition(Vector3(360.0f, 400.0f, 0.0f));
	//�t�H���g�̑傫����ݒ�B
	m_font.SetScale(1.0f);
}
