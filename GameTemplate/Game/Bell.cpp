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
}

Bell::Bell()
{
}
Bell::~Bell()
{
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
	g_soundEngine->ResistWaveFileBank(2, "Assets/sound/item/bell_low.wav");
	//���f���̏������B
	m_modelRender.Init("Assets/modelData/item/bell.tkm");
	//�T�E���h��NewGO
	m_bellSound = NewGO<SoundSource>(2);

	beforeRate = 0.00f;
	g_infoForEdge.SetRate(2, rate);

	g_infoForEdge.InitForSound(2, m_position, 200.0f, 0, rate);

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
	// 
	Font();
	// 
	//���f���̍X�V�B
	m_modelRender.Update();
	//g_infoForEdge.SetIsSound(2, m_bellSound->IsPlaying());
}

void Bell::Position()
{
	m_position = g_camera3D->GetPosition();
	m_position += g_camera3D->GetRight() * 20.0f;
	m_position += g_camera3D->GetForward() * 20.0f;
	m_position.y = 145.0f;
	//���W��ݒ肷��
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_player->GetRotation());
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
		m_bellSound->Init(2);
		m_bellSound->SetPosition(m_position);
		m_bellSound->SetVolume(VOLUME);
		m_bellSound->Play(false);

		g_infoForEdge.InitForSound(2, m_position, 400.0f, 0, VOLUME);
		
	}
	int check2;
	if (m_bellSound != nullptr) {
		if (m_bellSound->IsPlaying() == true)
		{
			check2 = 1;
			if (rate < 1.00f) {
				rate += 0.05f;
			}
		}
		else {
			check2 = 0;
			if (rate > 0.00f && check2 == 0) {
				rate -= 0.05f;
				if (rate <= 0.00f) {
					rate = 0.00f;
				}
			}
		}
		g_infoForEdge.SetIsSound(2, check2);
		g_infoForEdge.SetRate(2, rate);
	}


	m_timer -= g_gameTime->GetFrameDeltaTime();
	if (m_timer <= 0.0f)
	{
		m_bellSound = NewGO<SoundSource>(2);
		m_timer = 0.0f;
		TransitionState();
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