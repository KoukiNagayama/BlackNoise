#include "stdafx.h"
#include "Player.h"
#include "GameCamera.h"

//�萔
namespace
{
	const float MOVESPEED = 1000.0f;
	const float MOVE_RUN = 1.4f;
	const float MOVE_SNEAK = 0.58f;
}

//����������
bool Player::Start()
{

	m_gamecam = FindGO<GameCamera>("gamecamera");

	m_position = m_gamecam->GetPosition();
	m_position.y = 0.0f;

	m_modelRender.Init("Assets/modelData/human/playerNohead.tkm");
	m_modelRender.SetPosition(m_gamecam->GetPosition());

	//�L�����R��������������B
	m_charaCon.Init(10.0f, 45.0f, m_position);

	return true;
}

Player::~Player()
{

}

//�X�V����
void Player::Update()
{
	//�ړ�����
	Move();
	//��]�����B
	Rotation();
	//�X�e�[�g�Ǘ��B
	ManageState();	

	m_modelRender.Update();
}

void Player::Move()
{
	//FPS���_�̂��߁A�ړ�����W�̏��̓J�������ێ����Ă���B
	//�J�����̍��W���擾����B
	m_position = m_gamecam->GetPosition();
	//Y���W��0�ɂ���B
	m_position.y = 0.0f;
	//���W��ݒ肷��B
	m_modelRender.SetPosition(m_position);
}

void Player::Rotation()
{
	//�J�����̌������擾
	Vector3 rotVec = m_gamecam->GetToCameraPos();
	//y�̒l�̏��͗v��Ȃ��̂ŁA0.0f�������Đ��K���B
	rotVec.y = 0.0f;
	rotVec.Normalize();

	//���݂ǂꂾ����]���Ă���̂��̊p�x�����߂�B
	float angle = atan2(rotVec.x, rotVec.z);
	//���̊p�x���g���āAY������Quaternion��ݒ肷��B
	m_rotation.SetRotation(Vector3::AxisY, angle);

	//Quaternion���g���āA���f���̉�]��ݒ�
	m_modelRender.SetRotation(m_rotation);
}

void Player::TransitionState()
{
	//HP��0�ɂȂ�����
	if (m_hp == 0)
	{
		//�_�E����Ԃɂ���B
		m_playerState = enPlayerState_Down;
		return;
	}
	//x��z�̈ړ����x����������(�X�e�B�b�N�̓��͂���������)�B
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		//B�{�^���������Ă���Ԃ͑���B
		if (g_pad[0]->IsPress(enButtonB))
		{
			//�����Ԃɂ���B
			m_playerState = enPlayerState_Run;
			return;
		}
		//LB�{�^���������Ă���Ԃ��Ⴊ�ށB
		if (g_pad[0]->IsPress(enButtonLB2))
		{
			//���Ⴊ�ݏ�Ԃɂ���B
			m_playerState = enPlayerState_Sneak;
			return;
		}
		//������Ԃɂ���B
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
//�X�e�[�g�Ǘ�
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

//�`�揈��
void Player::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}