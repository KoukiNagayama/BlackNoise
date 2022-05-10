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
	const float RANGE = 800.0f;
	const float EDGE_FADE_IN_DELTA_VALUE = 0.07f;	// �G�b�W���t�F�[�h�C������Ƃ��̕ψʗ�
	const float EDGE_FADE_OUT_DELTA_VALUE = 0.0f;	// �G�b�W���t�F�[�h�A�E�g����Ƃ��̕ψʗ�
	const float RATE_BY_TIME_MAX_VALUE = 1.00f;		// ���Ԃɂ��e�����̍ő�l
	const float RATE_BY_TIME_MIN_VALUE = 0.00f;		// ���Ԃɂ��e�����̍ŏ��l
	const float MODEL_MULTIPLIER = 35.0f;
	const float MODEL_UP = 25.0f;
	const float SOUND_MULTIPLIER = 15.0f;
}

Bell::Bell()
{
}
Bell::~Bell()
{
	DeleteGO(m_bellSound);
}

bool Bell::Start()
{
	//�A�j���[�V�����̏������B
	animationClips[enAnimationClip_Idle].Load("Assets/animData/item/idle.tka");
	animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	animationClips[enAnimationClip_Ring].Load("Assets/animData/item/ring.tka");
	animationClips[enAnimationClip_Ring].SetLoopFlag(true);

	m_player = FindGO<Player>("player");
	m_gameCam = FindGO<GameCamera>("gamecamera");
	//�T�E���h��o�^�B
	g_soundEngine->ResistWaveFileBank(0, "Assets/sound/item/bell_low.wav");
	//���f���̏������B
	m_modelRender.Init("Assets/modelData/item/bell.tkm");
	//m_modelRender.SetScale({ 0.7f,0.7f,0.7f });

	//�T�E���h��NewGO
	m_bellSound = NewGO<SoundSource>(0);

	g_infoForEdge.InitForSound(0, m_position, RANGE, 0, rate);

	return true;
}

void Bell::Update()
{
	//���W
	Position();
	//�X�e�[�g�Ǘ�
	ManageState();
	//�A�j���[�V�����Đ��B
	//PlayAnimation();
	// �f�o�b�O�p�̕����\��
	Font();
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
		m_bellState = enBellState_Ring;
		m_timer = TIMER;
		return;
	}
	m_bellState = enBellState_Idle;
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
	if (m_timer >= TIMER) {
		m_bellSound->Init(0);
		m_bellSound->SetPosition(m_position);
		m_bellSound->SetVolume(VOLUME);
		m_bellSound->Play(false);
		
	}
	int check;
	if (m_bellSound != nullptr) {
		if (m_bellSound->IsPlaying() == true)
		{
			check = 1;
			if (rate < RATE_BY_TIME_MAX_VALUE) {
				rate += EDGE_FADE_IN_DELTA_VALUE;
			}
		}
		else {
			check = 0;
			if (rate > RATE_BY_TIME_MIN_VALUE && check == 0) {
				rate -= EDGE_FADE_OUT_DELTA_VALUE;
				if (rate <= RATE_BY_TIME_MIN_VALUE) {
					rate = RATE_BY_TIME_MIN_VALUE;
					TransitionState();
				}
			}
		}
		g_infoForEdge.SetInfoForSound(0, m_soundPos, RANGE, check, rate);
	}


	m_timer -= g_gameTime->GetFrameDeltaTime();
	if (m_timer <= 0.0f)
	{
		m_bellSound = NewGO<SoundSource>(0);
		m_timer = 0.0f;
	}
}

void Bell::Idle()
{
	TransitionState();
}

void Bell::PlayAnimation()
{
	switch (m_bellState)
	{
		//�ҋ@�B
	case enBellState_Idle:
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.3f);
		break;
		//����炷�B
	case enBellState_Ring:
		m_modelRender.PlayAnimation(enAnimationClip_Ring, 0.3f);
	default:
		break;
	}
}

void Bell::Font()
{
	wchar_t wcsbuf[256];
	swprintf_s(wcsbuf, 256, L"timer:%.3f", m_timer);
	//�\������e�L�X�g��ݒ�B
	m_font.SetText(wcsbuf);
	//�t�H���g�̈ʒu��ݒ�B
	m_font.SetPosition(Vector3(-760.0f, 400.0f, 0.0f));
	//�t�H���g�̑傫����ݒ�B
	m_font.SetScale(1.0f);
	swprintf_s(wcsbuf, 256, L"rate:%.3f", rate);
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
	m_font.Draw(rc);
	m_font1.Draw(rc);
}