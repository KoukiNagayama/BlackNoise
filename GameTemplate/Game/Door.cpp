#include "stdafx.h"
#include "Door.h"
#include "GameCamera.h"
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

namespace
{
	const float DISTANCE = 90.0f;
}

Door::Door()
{

}

Door::~Door()
{

}

bool Door::Start()
{
	//�A�j���[�V������ǂݍ��ށB
	m_animationClips[enAnimationClip_CloseIdle].Load("Assets/animData/door/closeidle.tka");
	m_animationClips[enAnimationClip_CloseIdle].SetLoopFlag(false);
	m_animationClips[enAnimationClip_OpenIdle].Load("Assets/animData/door/openidle.tka");
	m_animationClips[enAnimationClip_OpenIdle].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Open].Load("Assets/animData/door/open.tka");
	m_animationClips[enAnimationClip_Open].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Close].Load("Assets/animData/door/close.tka");
	m_animationClips[enAnimationClip_Close].SetLoopFlag(false);
	//���f����ǂݍ��ށB
	m_modelRender.Init("Assets/modelData/stage/door.tkm", m_animationClips, enAnimationClip_Num);
	//���W��ݒ肷��B
	m_modelRender.SetPosition(m_position);
	//�傫����ݒ肷��B
	m_modelRender.SetScale(m_scale);
	//��]��ݒ肷��B
	m_modelRender.SetRotation(m_rotation);

	//���f���̍X�V�B
	m_modelRender.Update();
	//�����蔻����쐬����B
	//m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());

	m_gamecam = FindGO<GameCamera>("gamecamera");

	//����ǂݍ��ށB
	//g_soundEngine->ResistWaveFileBank(5, "Assets/sound/door_cut.wav");
	return true;
}

void Door::Update()
{
	//�A�j���[�V�������Đ�����B
	PlayAnimation();
	//�X�e�[�g�Ǘ��B
	ManageState();

	NearPlayer();

	//���f���̍X�V�B
	m_modelRender.Update();
}
bool Door::NearPlayer()
{
	Vector3 cameraPos = m_gamecam->GetPosition();
	cameraPos.y = 0.0f;
	Vector3 disToPlayer = m_position - cameraPos;
	// ��苗���߂Â�����
	test = disToPlayer.Length();
	if (disToPlayer.Length() <= DISTANCE) {
		return true;
	}
}

void Door::TransitionState()
{
	test = test;
	if (NearPlayer()!=false && g_pad[0]->IsTrigger(enButtonA))
	{
		//���Ă���Ƃ���
		if (m_doorState = enDoorState_CloseIdle)
		{
			//�J����
			m_doorState = enDoorState_Open;
		}
		//�J���Ă���Ƃ���
		else if (m_doorState = enDoorState_OpenIdle)
		{
			//����
			m_doorState = enDoorState_Close;
		}
	}
}

void Door::PlayAnimation()
{
	switch (m_doorState)
	{
		//�I�[�v���X�e�[�g�̎��B
	case enDoorState_Open:
		//�I�[�v���A�j���[�V�������Đ�����B
		m_modelRender.PlayAnimation(enAnimationClip_Open);
		break;
		//�I�[�v���X�e�[�g�̎��B
	case enDoorState_OpenIdle:
		//�I�[�v���A�j���[�V�������Đ�����B
		m_modelRender.PlayAnimation(enAnimationClip_OpenIdle);
		break;
		//�N���[�Y�X�e�[�g�̎��B
	case enDoorState_Close:
		//�N���[�Y�A�j���[�V�������Đ�����B
		m_modelRender.PlayAnimation(enAnimationClip_Close);
		break;
		//�N���[�Y�X�e�[�g�̎��B
	case enDoorState_CloseIdle:
		//�N���[�Y�A�j���[�V�������Đ�����B
		m_modelRender.PlayAnimation(enAnimationClip_Close);
		break;
	default:
		break;
	}
}

void Door::ReleasePhysicsObject()
{
	//�����蔻����J������B
	m_physicsStaticObject.Release();
}

void Door::CreatePhysicsObject()
{
	//�����蔻����쐬����B
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());
}

void Door::OpenState()
{
	//�I�[�v���A�j���[�V�����̍Đ����I��������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�����蔻����J������B
		//ReleasePhysicsObject();
		//�I�[�v���I���X�e�[�g�ɑJ�ڂ���B
		m_doorState = enDoorState_OpenIdle;
	}
}
void Door::OpenIdleState()
{
	TransitionState();
}

void Door::CloseState()
{
	//�N���[�Y�A�j���[�V�������I��������B
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�����蔻����쐬����B
		//CreatePhysicsObject();
		//�N���[�Y�I���X�e�[�g�ɑJ�ڂ���B
		m_doorState = enDoorState_CloseIdle;
	}
}
void Door::CloseIdleState()
{
	TransitionState();
}



void Door::ManageState()
{
	switch (m_doorState)
	{
		//�I�[�v���X�e�[�g�̎��B
	case enDoorState_Open:
		//�I�[�v���X�e�[�g�̃X�e�[�g�J�ڏ����B
		OpenState();
		break;
		//�I�[�v���I���X�e�[�g�̎��B
	case enDoorState_OpenIdle:
		OpenIdleState();
		break;
		//�N���[�Y�X�e�[�g�̎��B
	case enDoorState_Close:
		//�N���[�Y�X�e�[�g�̃X�e�[�g�J�ڏ����B
		CloseState();
		break;
		//�N���[�Y�I���X�e�[�g�̎��B
	case enDoorState_CloseIdle:
		CloseIdleState();
		break;
	default:
		break;
	}
}

void Door::Render(RenderContext& rc)
{
	//���f����`�悷��B
	m_modelRender.Draw(rc);
}

