#include "stdafx.h"
#include "Player.h"

namespace
{
	const float MOVESPEED = 250.0f;
}

bool Player::Start()
{
	m_modelRender.Init("Assets/modelData/human/jackie.tkm", animationClips, enAnimationClip_Num);
	m_modelRender.Update();

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
}

void Player::Move()
{
	if (IsEnableMove() == false)
	{
		return;
	}
	//x,z�̈ړ����x��0�ɂ���B
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.y = 0.0f;

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
		m_moveSpeed.x *= 1.3f;
		m_moveSpeed.z *= 1.3f;
	}
	//���Ⴊ�݃X�e�[�g�Ȃ瑬�x��0.7�{�ɂ���B
	if (m_playerState == enPlayerState_Sneak)
	{
		m_moveSpeed.x *= 0.7f;
		m_moveSpeed.z *= 0.7f;
	}

	//�n�ʂɂ��Ă�����
	if (m_charaCon.IsOnGround())
	{
		//�d�͂��Ȃ����B
		m_moveSpeed.y = 0.0f;
	}
	//�n�ʂɂ��Ă��Ȃ�������
	else
	{
		//�d�͂𔭐�������B
		m_moveSpeed.y -= pow(m_moveSpeed.y, 9.8f);
	}

	//�L�����R�����g���č��W���ړ�������B
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	//���W��ݒ肷��B
	m_modelRender.SetPosition(m_position);
}

void Player::Rotation()
{
	//�ړ����Ă��Ȃ��Ƃ��͉�]���Ȃ�
	if (fabsf(m_moveSpeed.x) < 0.001f
		&& fabsf(m_moveSpeed.z) < 0.001f) {
		return;
	}
	//��]�p�x���v�Z����B
	float angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);
	m_rotation.SetRotationY(-angle);

	//��]��ݒ肷��B
	m_modelRender.SetRotation(m_rotation);

	//�v���C���[�̐��ʃx�N�g�����v�Z����B
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}