#include "stdafx.h"
#include "TitleGramophone.h"
#include "Title.h"
#include "InfoForEdge.h"
#include "sound/SoundEngine.h"
#include "TitleCamera.h"

namespace
{
	const float RANGE = 700.0f;						// �e���͈�
	const float EDGE_FADE_IN_DELTA_VALUE = 0.01f;	// �G�b�W���t�F�[�h�C������Ƃ��̕ψʗ�
	const float EDGE_FADE_OUT_DELTA_VALUE = 0.01f;	// �G�b�W���t�F�[�h�A�E�g����Ƃ��̕ψʗ�
	const float RATE_BY_TIME_MAX_VALUE = 1.00f;		// ���Ԃɂ��e�����̍ő�l
	const float RATE_BY_TIME_MIN_VALUE = 0.00f;		// ���Ԃɂ��e�����̍ŏ��l
	const float MAXIMUM_VOLUME = 2.00f;				// �ő剹��
	const float MINIMUM_VOLUME = 0.00f;				// �ŏ�����
}
TitleGramophone::~TitleGramophone()
{

}
bool TitleGramophone::Start()
{
	// ������
	Init();

	// �Q�[���J����
	m_titleCamera = FindGO<TitleCamera>("titleCamera");

	return true;
}

void TitleGramophone::Init()
{
	// �����͑ҋ@���
	m_gramophoneState = enGramophoneState_Idle;
	m_modelRender.Init("Assets/modelData/item/record_on.tkm", nullptr, 0, false, enModelUpAxisZ, false, 0, 1);
	// �T�E���h��o�^
	g_soundEngine->ResistWaveFileBank(14, "Assets/sound/record/record5.wav");
	g_infoForEdge.InitForSound(0, m_position, RANGE, 0, m_rateByTime);

	// ���f���̍��W�A��]�A�g���ݒ�
	m_modelRender.SetTRS(m_position, m_rotation, m_scale);

	// �^�C�g��
	m_title = FindGO<Title>("title");

}

void TitleGramophone::Update()
{
	m_titleCamera = FindGO<TitleCamera>("titleCamera");

	// �X�e�[�g�Ǘ�
	ManegeState();

	m_modelRender.Update();
}

void TitleGramophone::ManegeState()
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


void TitleGramophone::IdleState()
{
	// �^�C�g���̃X�e�[�g���ҋ@���Ȃ��
	if (m_title->GetTitleState() != 2) {
		return;
	}
	// �^�C�g���p�~���@�̃X�e�[�g�����t���ɕύX����
	m_gramophoneState = enGramophoneState_Play;
}

void TitleGramophone::PlayState()
{
	if (m_soundSource1 == nullptr) {
		// ����炷
		MakeSound();
	}

	// ���ʒ���
	VolumeControl();

	// �e����
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

	//�v���C���[�ƒ~���@�̋���
	Vector3 diff = m_position - gameCameraPos;

	// ���̑傫��
	float soundLevel = MAXIMUM_VOLUME - (diff.Length() / range * MAXIMUM_VOLUME);
	// �ŏ����ʂ�菬�����Ȃ�����Œ肷��
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
		// �������Đ���
		if (m_soundSource1->IsPlaying() == true)
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
		g_infoForEdge.SetIsSound(0, check);
		g_infoForEdge.SetRate(0, m_rateByTime);
	}
	
}

void TitleGramophone::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}


