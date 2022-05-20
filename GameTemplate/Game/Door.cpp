#include "stdafx.h"
#include "Door.h"
#include "GameCamera.h"
#include "Enemy.h"
#include "sound/SoundEngine.h"

namespace
{
	const float DISTANCE = 130.0f;					//�v���C���[�̋���
	const float EDGE_FADE_IN_DELTA_VALUE = 0.07f;	// �G�b�W���t�F�[�h�C������Ƃ��̕ψʗ�
	const float EDGE_FADE_OUT_DELTA_VALUE = 0.005f;	// �G�b�W���t�F�[�h�A�E�g����Ƃ��̕ψʗ�
	const float RATE_BY_TIME_MAX_VALUE = 1.00f;		// ���Ԃɂ��e�����̍ő�l
	const float RATE_BY_TIME_MIN_VALUE = 0.00f;		// ���Ԃɂ��e�����̍ŏ��l
	const float SOUND_RANGE = 300.0f;				//�e������͈�
	const float ADD_DEG = 4.0f;						//1�t���[���ŉ��Z����p�x
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
	m_modelRender.Init("Assets/modelData/stage/door.tkm");
	//���W��ݒ肷��B
	m_modelRender.SetPosition(m_position);
	//�傫����ݒ肷��B
	m_modelRender.SetScale(m_scale);
	//��]��ݒ肷��B
	m_modelRender.SetRotation(m_rotation);

	//���f���̍X�V�B
	m_modelRender.Update();
	//�����蔻����쐬����B
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());

	//���̓ǂݍ���
	g_soundEngine->ResistWaveFileBank(12, "Assets/sound/door/door_open.wav");
	g_soundEngine->ResistWaveFileBank(13, "Assets/sound/door/door_close.wav");
	//�֊s���`�ʂ̏�����
	g_infoForEdge.InitForSound(8, m_position, SOUND_RANGE, 0, m_rateByTime);

	m_gamecam = FindGO<GameCamera>("gamecamera");
	m_enemy = FindGO<Enemy>("enemy");
	return true;
}

void Door::Update()
{
	//�A�j���[�V�������Đ�����B
	PlayAnimation();
	//�X�e�[�g�Ǘ��B
	ManageState();
	//�h�A�Ƃ̋���
	NearPlayer();
	//�h�A�Ƃ̋���(Enemy)
	NearEnemy();
	//�e����
	CheckRate();

	//���f���̍X�V�B
	m_modelRender.Update();
}
bool Door::NearPlayer()
{
	Vector3 cameraPos = m_gamecam->GetPosition();
	cameraPos.y = 0.0f;
	Vector3 disToPlayer = m_position - cameraPos;
	// ��苗���߂Â�����
	if (disToPlayer.Length() <= DISTANCE) {
		return true;
	}
	return false;
}

bool Door::NearEnemy()
{
	Vector3 enemyPos = m_enemy->GetPosition();
	enemyPos.y = 0.0f;
	Vector3 disToPlayer = m_position - enemyPos;
	// ��苗���߂Â�����
	if (disToPlayer.Length() <= DISTANCE) {
		return true;
	}
	return false;
}

void Door::TransitionState()
{
	//�h�A�󂹂��Ԃ̂Ƃ��AA�{�^����������
	if (NearPlayer() != false && g_pad[0]->IsTrigger(enButtonA))
	{
		//���܂��Ă���Ƃ�
		if (m_doorState == enDoorState_CloseIdle)
		{
			MakeSound(0);
			//�h�A���J����
			m_doorState = enDoorState_Open;
		}
		//�J���Ă���Ƃ�
		else if (m_doorState == enDoorState_OpenIdle)
		{
			m_close = false;
			//�h�A�����
			m_doorState = enDoorState_Close;
		}
	}
	else if (NearEnemy() != false)
	{
		//���܂��Ă���Ƃ�
		if (m_doorState == enDoorState_CloseIdle)
		{
			MakeSound(0);
			//�h�A���J����
			m_doorState = enDoorState_Open;
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
void Door::MakeSound(int number)
{
	switch (number)
	{
		//�J����
	case 0:
		m_sound = NewGO<SoundSource>(12);
		m_sound->Init(12);
		m_sound->SetVolume(1.0f);
		m_sound->Play(false);
		break;

		//���鉹
	case 1:
		m_sound = NewGO<SoundSource>(13);
		m_sound->Init(13);
		m_sound->SetVolume(1.0f);
		m_sound->Play(false);
		break;
	default:
		break;
	}
}

void Door::CheckRate()
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
void Door::OpenIdleState()
{
	TransitionState();
}

void Door::CloseState()
{
	ReleasePhysicsObject();
	//if(PlayAnimation)

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
void Door::CloseIdleState()
{
	if (m_close != true) {
		MakeSound(1);
		m_close = true;
	}
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

