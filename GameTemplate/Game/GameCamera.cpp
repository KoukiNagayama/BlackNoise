#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"

namespace
{
	const float CAMPOS_Y = 170.0f;	//���_�̍���
	const float TOCAMPOS_Z = -77.0f;			//�����_�܂ł�Z���W

	const float MOVESPEED = 140.0f;			//�����̈ړ����x
	const float MOVE_RUN = 10.8f;			//���莞�ɂ������Z���邩
	const float MOVE_SNEAK = 0.3f;			//���Ⴊ�ݎ��ɂ������Z���邩
	const float MOVE_FRICTION = 30.0f;
}

GameCamera::GameCamera()
{
}
GameCamera::~GameCamera()
{
}
bool GameCamera::Start()
{
	//�����_���王�_�܂ł̃x�N�g����ݒ�B
	m_toCameraPos.Set(0.0f, 0.0f, TOCAMPOS_Z);
	
	////////////////////////�f�o�b�O�J����//////////////////////
	//m_toCameraPos.Set(0.0f, CAMPOS_TARGET_Y, -250.0f);
	////////////////////////�f�o�b�O�J����//////////////////////

	//�v���C���[�̃C���X�^���X��T���B
	m_player = FindGO<Player>("player");

	m_position = Vector3::Zero;

	//�J�����̃j�A�N���b�v�ƃt�@�[�N���b�v��ݒ肷��B
	g_camera3D->SetNear(3.0f);
	g_camera3D->SetFar(10000.0f);

	//�L�����R��������������B
	m_charaCon.Init(1.0f, 1.0f, m_position);

	return true;
}
void GameCamera::Update()
{
	//�ړ�����
	Move();
	//�����_�̏���
	ViewPoint();
	//�X�e�[�g�J�ڏ���
	TransitionState();

	//�J�����̍X�V�B
	g_camera3D->Update();
}

void GameCamera::Move()
{
	if (m_player->IsEnableMove() == false)
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
	if (m_moveState == enMoveState_Run)
	{
		m_moveSpeed.x *= MOVE_RUN;
		m_moveSpeed.z *= MOVE_RUN;
	}
	//���Ⴊ�݃X�e�[�g�Ȃ瑬�x��0.7�{�ɂ���B
	if (m_moveState == enMoveState_Sneak)
	{
		m_moveSpeed.x *= MOVE_SNEAK;
		m_moveSpeed.z *= MOVE_SNEAK;
	}
	
	//�L�����R�����g���č��W���ړ�������B
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	//���_�̍������グ��B
	m_position.y = CAMPOS_Y;
	//���Ⴊ�ݏ�Ԃ�������
	if (IsSneak() == false)
	{
		m_position.y -= m_position.y / 2;
	}
	//�J�����̍��W��ݒ肷��B
	g_camera3D->SetPosition(m_position);
}
//�����_�̏���
void GameCamera::ViewPoint()
{
	//�J�������X�V�B
	//�����_���v�Z����B
	m_target = m_position;
	//�v���C���̑������炿����Ə�𒍎��_�Ƃ���B
	m_target += g_camera3D->GetForward() * 50.0f;

	Vector3 toCameraPosOld = m_toCameraPos;
	//�p�b�h�̓��͂��g���ăJ�������񂷁B
	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();
	//Y������̉�]
	Quaternion qRot;
	qRot.SetRotationDeg(Vector3::AxisY, 1.3f * x);
	qRot.Apply(m_toCameraPos);

	//X������̉�]�B
	Vector3 axisX;
	axisX.Cross(Vector3::AxisY, m_toCameraPos);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX, 1.3f * -y);
	qRot.Apply(m_toCameraPos);

	//�J�����̉�]�̏�����`�F�b�N����B
	//�����_���王�_�܂ł̃x�N�g���𐳋K������B
	Vector3 toPosDir = m_toCameraPos;
	toPosDir.Normalize();
	if (toPosDir.y < -0.99f) {
		//�J����������������B
		m_toCameraPos = toCameraPosOld;
	}
	else if (toPosDir.y > 0.9f) {
		//�J�����������������B
		m_toCameraPos = toCameraPosOld;
	}

	//���_���v�Z����B
	m_target = m_position + m_toCameraPos;
	//���C���J�����ɒ����_�Ǝ��_��ݒ肷��B
	g_camera3D->SetTarget(m_target);
}

void GameCamera::TransitionState()
{

	//x��z�̈ړ����x����������(�X�e�B�b�N�̓��͂���������)�B
	if (IsMoveNow()==true)
	{
		//B�{�^���������Ă���Ԃ͑���B
		if (g_pad[0]->IsPress(enButtonB))
		{
			//�����Ԃɂ���B
			m_moveState = enMoveState_Run;
			return;
		}
		//LB�{�^���������Ă���Ԃ��Ⴊ�ށB
		if (g_pad[0]->IsPress(enButtonLB2))
		{
			//���Ⴊ�ݏ�Ԃɂ���B
			m_moveState = enMoveState_Sneak;
			return;
		}
		//������Ԃɂ���B
		m_moveState = enMoveState_Walk;
	}
	else
	{
		//LB2�������Ă�����
		if (g_pad[0]->IsPress(enButtonLB2))
		{
			//���Ⴊ�ݑҋ@��Ԃɂ���B
			m_moveState = enMoveState_SneakIdle;
			return;
		}
		//�ҋ@��Ԃɂ���B
		m_moveState = enMoveState_Idle;
	}
}