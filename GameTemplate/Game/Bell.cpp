#include "stdafx.h"
#include "Bell.h"
#include "sound/SoundEngine.h"
#include "Player.h"
#include "GameCamera.h"
#include "InfoForEdge.h"

namespace
{
	const float POSITION = 60.0f;
	const float TIMER = 3.0f;
	const float VOLUME = 1.0f;
<<<<<<< HEAD
	const float SOUND_RANGE = 700.0f;
=======
	const float SOUND_RANGE = 800.0f;
>>>>>>> b86fa71a70d73fdbfa0029f2dae1823d616d5e36
	const float EDGE_FADE_IN_DELTA_VALUE = 0.07f;	// �G�b�W���t�F�[�h�C������Ƃ��̕ψʗ�
	const float EDGE_FADE_OUT_DELTA_VALUE = 0.01f;	// �G�b�W���t�F�[�h�A�E�g����Ƃ��̕ψʗ�
	const float RATE_BY_TIME_MAX_VALUE = 1.00f;		// ���Ԃɂ��e�����̍ő�l
	const float RATE_BY_TIME_MIN_VALUE = 0.00f;		// ���Ԃɂ��e�����̍ŏ��l
	const float MODEL_MULTIPLIER = 35.0f;			//�J�����̑O�����A�E�����ɏ�Z����l�B
	const float MODEL_UP = 25.0f;					//�J�����̏�����ɏ�Z����l
	const float SOUND_MULTIPLIER = 15.0f;			//�x���̑O�����ɏ�Z����l

}

Bell::Bell()
{
}
Bell::~Bell()
{
	g_infoForEdge.SetIsSound(0, 0);
	g_infoForEdge.SetRate(0, 0.00f);
	if (m_bellSound != nullptr) {
		DeleteGO(m_bellSound);
	}
}

bool Bell::Start()
{
	m_player = FindGO<Player>("player");
	m_gameCam = FindGO<GameCamera>("gamecamera");
	//�T�E���h��o�^�B
	g_soundEngine->ResistWaveFileBank(0, "Assets/sound/item/bell_low.wav");
	//���f���̏������B
	m_modelRender.Init("Assets/modelData/item/bell.tkm");

	g_infoForEdge.InitForSound(0, m_position, SOUND_RANGE, 0, m_rateByTime);

	return true;
}

void Bell::Update()
{
	//���W
	Position();
	//�X�e�[�g�Ǘ�
	ManageState();
	// �f�o�b�O�p�̕����\��
	Font();
	//�e����
	CheckRate();
	//���f���̍X�V�B
	m_modelRender.Update();
}

void Bell::Position()
{
	Vector3 up, right, forward;
	right = g_camera3D->GetRight();
	forward = g_camera3D->GetForward();
	up.Cross(right, forward);
	up.Normalize();
	//�x���̃��f���̍��W���v�Z����B
	m_position = g_camera3D->GetPosition();
	m_position += g_camera3D->GetRight() * MODEL_MULTIPLIER;
	m_position += g_camera3D->GetForward() * MODEL_MULTIPLIER;
	m_position += up * MODEL_UP;
	//���W��ݒ肷��
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_player->GetRotation());

	//�x���̉��̍��W�������O�ɐݒ肷��B
	m_soundPos = g_camera3D->GetPosition();
	m_soundPos += g_camera3D->GetForward() * SOUND_MULTIPLIER;

}

void Bell::TransitionState()
{
	if (g_pad[0]->IsTrigger(enButtonB))
	{
		//����炷
		MakeSound();
		//Ring�X�e�[�g�ɂ���
		m_bellState = enBellState_Ring;
		//�N�[���^�C���̃Z�b�g
		m_timer = TIMER;
		m_isRing = true;
		return;
	}
	m_bellState = enBellState_Idle;
	m_isRing = false;
}

void Bell::ManageState()
{
	switch (m_bellState)
	{
		//�ҋ@�B
	case enBellState_Idle:
		Idle();
		break;
		//����炷�B
	case enBellState_Ring:
		Ring();
	default:
		break;
	}
}

void Bell::Ring()
{
	if (m_timer <= 0.0f)
	{
		m_timer = 0.0f;
		m_bellState = enBellState_Idle;
		return;
	}
	m_timer -= g_gameTime->GetFrameDeltaTime();
}

void Bell::Idle()
{
	TransitionState();
}

void Bell::MakeSound()
{
	m_bellSound = NewGO<SoundSource>(0);
	m_bellSound->Init(0);
	m_bellSound->SetVolume(1.0f);
	m_bellSound->Play(false);
}

void Bell::CheckRate()
{
	int check1;
	if (m_bellSound != nullptr) {
		if (m_bellSound->IsPlaying() == true)
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
					m_bellSound = nullptr;
				}
			}
		}
		g_infoForEdge.SetInfoForSound(0, m_soundPos, SOUND_RANGE, check1, m_rateByTime);
	}
}

void Bell::Font()
{
	int i = 0;
	if (m_bellSound != nullptr)
	{
		i = 1;
	}
	wchar_t wcsbuf[256];
	swprintf_s(wcsbuf, 256, L"timer:%d", i);
	//�\������e�L�X�g��ݒ�B
	m_font.SetText(wcsbuf);
	//�t�H���g�̈ʒu��ݒ�B
	m_font.SetPosition(Vector3(-760.0f, 400.0f, 0.0f));
	//�t�H���g�̑傫����ݒ�B
	m_font.SetScale(1.0f);
	swprintf_s(wcsbuf, 256, L"rate:%.3f", m_rateByTime);
	//�\������e�L�X�g��ݒ�B
	m_font1.SetText(wcsbuf);
	//�t�H���g�̈ʒu��ݒ�B
	m_font1.SetPosition(Vector3(-760.0f, 300.0f, 0.0f));
	//�t�H���g�̑傫����ݒ�B
	m_font1.SetScale(1.0f);
}

void Bell::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
	//m_font.Draw(rc);
	//m_font1.Draw(rc);
}