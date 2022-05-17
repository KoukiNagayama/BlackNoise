#include "stdafx.h"
#include "BrokenDoor.h"
#include "GameCamera.h"
#include "sound/SoundEngine.h"

namespace {
	const float DISTANCE = 150.0f;					//�h�A�Ƃ̋���
	const float EDGE_FADE_IN_DELTA_VALUE = 0.07f;	// �G�b�W���t�F�[�h�C������Ƃ��̕ψʗ�
	const float EDGE_FADE_OUT_DELTA_VALUE = 0.005f;	// �G�b�W���t�F�[�h�A�E�g����Ƃ��̕ψʗ�
	const float RATE_BY_TIME_MAX_VALUE = 1.00f;		// ���Ԃɂ��e�����̍ő�l
	const float RATE_BY_TIME_MIN_VALUE = 0.00f;		// ���Ԃɂ��e�����̍ŏ��l
	const float SOUND_RANGE = 900.0f;				//�e������͈�
	const float ADD_DEG = 4.0f;						//1�t���[���ŉ��Z����p�x
}

bool BrokenDoor::Start()
{
	//�A�j���[�V������ǂݍ��ށB
	m_animationClips[enAnimationClip_Before].Load("Assets/animData/door/closeidle.tka");
	m_animationClips[enAnimationClip_Before].SetLoopFlag(true);
	m_animationClips[enAnimationClip_CloseIdle].Load("Assets/animData/brokendoor/closeidle.tka");
	m_animationClips[enAnimationClip_CloseIdle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_OpenIdle].Load("Assets/animData/brokendoor/openidle.tka");
	m_animationClips[enAnimationClip_OpenIdle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Open].Load("Assets/animData/brokendoor/open.tka");
	m_animationClips[enAnimationClip_Open].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Close].Load("Assets/animData/brokendoor/close.tka");
	m_animationClips[enAnimationClip_Close].SetLoopFlag(false);
	//���f����ǂݍ��ށB
	m_modelRender.Init("Assets/modelData/stage/door.tkm");
	//���W��ݒ肷��B
	m_modelRender.SetPosition(m_position);
	//��]��ݒ肷��B
	m_modelRender.SetRotation(m_rotation);
	//���f���̍X�V�B
	m_modelRender.Update();
	//�����蔻����쐬����B
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());
	//�Q�[���J����
	m_gamecam = FindGO<GameCamera>("gamecamera");
	//�j�󉹂̓ǂݍ���
	g_soundEngine->ResistWaveFileBank(9, "Assets/sound/door/door_break.wav");
	//�֊s���`�ʂ̏�����
	g_infoForEdge.InitForSound(5, m_position, SOUND_RANGE, 0, m_rateByTime);

	return true;
}

void BrokenDoor::Update()
{
	//�X�e�[�g�Ǘ�
	ManageState();
	//�A�j���[�V�����Đ�
	PlayAnimation();
	//�h�A����߂���
	NearDoor();
	//�e����
	CheckRate();
	//���f���X�V
	m_modelRender.Update();
}

void BrokenDoor::MakeSound()
{
	m_sound = NewGO<SoundSource>(9);
	m_sound->Init(9);
	m_sound->SetVolume(1.0f);
	m_sound->Play(false);
}

void BrokenDoor::CheckRate()
{
	int check1;
	if (m_sound != nullptr) {
		if (m_sound->IsPlaying() == true)
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
				}
			}
		}
		g_infoForEdge.SetInfoForSound(5, m_position, SOUND_RANGE, check1, m_rateByTime);
	}
}
bool BrokenDoor::NearDoor()
{
	Vector3 disToPlayer = m_position - m_gamecam->GetYaxisZeroPosition();
	if (disToPlayer.Length() <= DISTANCE)
	{
		return true;
	}
}

void BrokenDoor::ManageState()
{
	switch (m_doorState)
	{
		//�󂷑O
	case enDoorState_Before:
		BeforeState();
		break;
		//�N���[�Y
	case enDoorState_Close:
		CloseState();
		break;
		//�N���[�Y�ҋ@
	case enDoorState_CloseIdle:
		CloseIdleState();
		break;
		//�I�[�v��
	case enDoorState_Open:
		OpenState();
		break;
		//�I�[�v���ҋ@
	case enDoorState_OpenIdle:
		OpenIdleState();
		break;
	default:
		break;
	}
}
void BrokenDoor::CreatePhysicsObject()
{
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());
}

void BrokenDoor::ReleasePhysicsObject()
{
	m_physicsStaticObject.Release();
}

bool BrokenDoor::CanBreakDoor()
{
	//�n���}�[�������Ă��āA�h�A�ɋ߂���
	if (m_haveHammer != false /*&& NearDoor() != false*/)
	{
		if (NearDoor() != false) {
			//�󂹂�
			return true;
		}
	}
}

void BrokenDoor::TransitionState()
{
	//�󂷑O�̂Ƃ�
	if (m_doorState == enDoorState_Before)
	{
		//�����������Ȃ�
		return;
	}
	//�h�A�󂹂��Ԃ̂Ƃ��AA�{�^����������
	if (NearDoor() != false && g_pad[0]->IsTrigger(enButtonA))
	{
		//���܂��Ă���Ƃ�
		if (m_doorState == enDoorState_CloseIdle)
		{
			//�h�A���J����
			m_doorState = enDoorState_Open;
		}
		//�J���Ă���Ƃ�
		else if (m_doorState == enDoorState_OpenIdle)
		{
			//�h�A�����
			m_doorState = enDoorState_Close;
		}
	}
}

void BrokenDoor::BeforeState()
{
	if (CanBreakDoor() != true)
	{
		return;
	}
	if (g_pad[0]->IsTrigger(enButtonX))
	{
		//���f���������ւ���
		m_modelRender.Init("Assets/modelData/stage/brokendoor.tkm");

		//�����o��
		MakeSound();
		
		//�X�e�[�g���N���[�Y��Ԃɂ���
		m_doorState = enDoorState_CloseIdle;
		
		ReleasePhysicsObject();
	}
}

void BrokenDoor::CloseIdleState()
{
	TransitionState();
}

void BrokenDoor::CloseState()
{
	ReleasePhysicsObject();

	if (m_deg >= 0.0f)
	{
		m_rotation.AddRotationDegY(-ADD_DEG);
		m_modelRender.SetRotation(m_rotation);
		m_deg -= ADD_DEG;
	}
	else
	{
		CreatePhysicsObject();
		//�N���[�Y�I���X�e�[�g�ɑJ�ڂ���B
		m_doorState = enDoorState_CloseIdle;
	}

}
void BrokenDoor::OpenState()
{
	ReleasePhysicsObject();
	//100�x��]������
	if (m_deg <= 200.0f) 
	{
		m_rotation.AddRotationDegY(ADD_DEG);
		m_modelRender.SetRotation(m_rotation);
		m_deg += ADD_DEG;
	}
	else
	{
		CreatePhysicsObject();
		//�N���[�Y�I���X�e�[�g�ɑJ�ڂ���B
		m_doorState = enDoorState_OpenIdle;
	}

}

void BrokenDoor::OpenIdleState()
{
	TransitionState();
}

void BrokenDoor::PlayAnimation()
{
	switch (m_doorState)
	{
		//�󂷑O�̎�
	case enDoorState_Before:
		//�󂷑O�̃h�A�A�j���[�V�������Đ�
		m_modelRender.PlayAnimation(enAnimationClip_Before);
		break;
		//�I�[�v���X�e�[�g
	case enDoorState_Open:
		//�I�[�v���A�j���[�V�������Đ�
		m_modelRender.PlayAnimation(enAnimationClip_Open);
		break;
		//�I�[�v���ҋ@�X�e�[�g
	case enDoorState_OpenIdle:
		//�I�[�v���A�j���[�V�������Đ�
		m_modelRender.PlayAnimation(enAnimationClip_OpenIdle);
		break;
		//�N���[�Y�X�e�[�g
	case enDoorState_Close:
		//�N���[�Y�A�j���[�V�������Đ�
		m_modelRender.PlayAnimation(enAnimationClip_Close);
		break;
		//�N���[�Y�ҋ@�X�e�[�g
	case enDoorState_CloseIdle:
		//�N���[�Y�ҋ@�A�j���[�V�������Đ�
		m_modelRender.PlayAnimation(enAnimationClip_Close);
		break;
	default:
		break;
	}
}

void BrokenDoor::Render(RenderContext& rc)
{
	//if(m_doorState == enDoorState_Before || m_doorState != enDoorState_CloseIdle)
	m_modelRender.Draw(rc);
}