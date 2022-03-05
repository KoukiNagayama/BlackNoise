#include "stdafx.h"
#include "Player.h"

namespace
{
	const float MOVESPEED = 70.0f;
	const float MOVE_RUN = 1.4f;
	const float MOVE_SNEAK = 0.58f;
}

bool Player::Start()
{
	m_modelRender.Init("Assets/modelData/human/jackie.tkm");
	m_modelRender.SetPosition(m_position);

	//�L�����R��������������B
	m_charaCon.Init(10.0f, 45.0f, m_position);

	return true;
}

Player::~Player()
{

}

void Player::Update()
{
	//�ړ������B
	Move();
	//��]�����B
	Rotation();
	//�X�e�[�g�J�ڏ����B
	ManageState();	

	m_modelRender.Update();
}

void Player::Move()
{
	if (IsEnableMove() == false)
	{
		return;
	}
	//x,z�̈ړ����x��0�ɂ���B
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;

	//���X�e�B�b�N�̓��͗ʂ��v�Z
	Vector3 stickL;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.y = g_pad[0]->GetLStickYF();

	//�J�����̑O�����ƉE�����̃x�N�g�����擾�B
	Vector3 forward = g_camera3D->GetForward();
	Vector3 right = g_camera3D->GetRight();

	//Y�����ɂ͈ړ������Ȃ�
	forward.y = 0.0f;
	right.y = 0.0f;

	//�ړ����x�ɃX�e�B�b�N�̓��͗ʂ����Z����B
	m_moveSpeed += right * stickL.x * MOVESPEED;
	m_moveSpeed += forward * stickL.y * MOVESPEED;

	//����X�e�[�g�Ȃ瑬�x��1.3�{�ɂ���B
	if (m_playerState == enPlayerState_Run)
	{
		m_moveSpeed.x *= MOVE_RUN;
		m_moveSpeed.z *= MOVE_RUN;
	}
	//���Ⴊ�݃X�e�[�g�Ȃ瑬�x��0.7�{�ɂ���B
	if (m_playerState == enPlayerState_Sneak)
	{
		m_moveSpeed.x *= MOVE_SNEAK;
		m_moveSpeed.z *= MOVE_SNEAK;
	}
	//�n�ʂɂ��Ă�����
	if (m_charaCon.IsOnGround())
	{
		//�d�͂��Ȃ����B
		m_moveSpeed.y = 0.0f;
	}
	//�L�����R�����g���č��W���ړ�������B
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	//���W��ݒ肷��B
	m_modelRender.SetPosition(m_position);
}

void Player::Rotation()
{

	//��]�p�x���v�Z����B
	float angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);
	m_rotation.SetRotationY(-angle);

	//��]��ݒ肷��B
	m_modelRender.SetRotation(m_rotation);

	//�v���C���[�̐��ʃx�N�g�����v�Z����B
	m_rotation.Apply(m_forward);
}

void Player::TransitionState()
{
	//HP��0�ɂȂ�����
	if (m_hp == 0)
	{
		m_playerState = enPlayerState_Down;
		return;
	}
	//x��z�̈ړ����x����������(�X�e�B�b�N�̓��͂���������)�B
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		//B�{�^���������Ă���Ԃ͑���B
		if (g_pad[0]->IsPress(enButtonB))
		{
			m_playerState = enPlayerState_Run;
			return;
		}
		//LB�{�^���������Ă���Ԃ��Ⴊ�ށB
		if (g_pad[0]->IsPress(enButtonLB2))
		{
			m_playerState = enPlayerState_Sneak;
			return;
		}
		m_playerState = enPlayerState_Walk;
	}
	else
	{
		//�~�܂�O�����Ⴊ�ݏ�Ԃ�������
		if (m_playerState = enPlayerState_SneakIdle)
		{
			m_playerState = enPlayerState_SneakIdle;
			return;
		}
		m_playerState = enPlayerState_Idle;
	}
}

void Player::ManageState()
{
	switch (m_playerState)
	{
		//�ҋ@�X�e�[�g
	case enPlayerState_Idle:
		IdleState();
		break;
		//�����X�e�[�g
	case enPlayerState_Walk:
		WalkState();
		break;
		//����X�e�[�g
	case enPlayerState_Run:
		RunState();
		break;
		//���Ⴊ�ݏ��
	case enPlayerState_Sneak:
		SneakState();
		break;
		//���Ⴊ�ݑҋ@���
	case enPlayerState_SneakIdle:
		SneakIdleState();
		break;
		//�_���[�W���
	case enPlayerState_Damage:
		DamageState();
		break;
		//�_�E�����
	case enPlayerState_Down:
		DownState();
		break;
	default:
		break;
	}
}
//�ҋ@�X�e�[�g���̏���
void Player::IdleState()
{
	TransitionState();
}
//�����X�e�[�g���̏���
void Player::WalkState()
{
	TransitionState();
}
//����X�e�[�W�̏���
void Player::RunState()
{
	TransitionState();
}
//���Ⴊ�݃X�e�[�g���̏���
void Player::SneakState()
{
	TransitionState();
}
//���Ⴊ�ݑҋ@�X�e�[�g���̏���
void Player::SneakIdleState()
{
	TransitionState();
}
//�_���[�W�X�e�[�g���̏���
void Player::DamageState()
{
	TransitionState();
}
//�_�E���X�e�[�g���̏���
void Player::DownState()
{
	TransitionState();
}

void Player::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}