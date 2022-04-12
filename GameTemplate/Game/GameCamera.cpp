#include "stdafx.h"
#include "sound/SoundEngine.h"
#include "GameCamera.h"
#include "Player.h"
#include "InfoForEdge.h"

namespace
{
	const float CAMPOS_Y = 250.0f;			//���_�̍���
	const float TOCAMPOS_Z = -77.0f;		//�����_�܂ł�Z���W

	const float MOVESPEED = 300.0f;			//�����̈ړ����x
	const float MOVE_RUN = 1.22f;			//���莞�ɂ������Z���邩
	const float MOVE_SNEAK = 0.3f;			//���Ⴊ�ݎ��ɂ������Z���邩
	const float TARGET_UNDER = -0.99f;		//�J�����̉���
	const float TARGET_OVER = 0.35f;		//�J�����̏��
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

	//�v���C���[�̃C���X�^���X��T���B
	m_player = FindGO<Player>("player");

	//m_position = Vector3::Zero;
	m_modelRender.Init("Assets/modelData/human/playerbox.tkm");
	m_modelRender.SetPosition(m_position);

	//�J�����̃j�A�N���b�v�ƃt�@�[�N���b�v��ݒ肷��B
	g_camera3D->SetNear(3.0f);
	g_camera3D->SetFar(10000.0f);

	//�L�����R��������������B
	m_charaCon.Init(50.0f, 170.0f, m_position);

	//�T�E���h��o�^�B
	g_soundEngine->ResistWaveFileBank(3, "Assets/sound/human/walk.wav");
	//�T�E���h��NewGO
	m_walkSound = NewGO<SoundSource>(3);

	beforeRate = 0.00f;
	g_infoForEdge.SetRate(2, rate);
	g_infoForEdge.InitForSound(2, m_position, 200.0f, 0, rate);

	return true;
}
void GameCamera::Update()
{
	//�ړ�����
	Move();
	//�����_�̏���
	ViewPoint();
	//�X�e�[�g�J�ڏ���
	ManageState();

	Vector3 position;
	position = m_position;
	position.y = 0.0f;
	m_modelRender.SetPosition(position);
	m_modelRender.Update();

	g_soundEngine->SetListenerPosition(m_position);
	g_soundEngine->SetListenerFront(g_camera3D->GetForward());
	//�J�����̍X�V�B
	g_camera3D->Update();
}

void GameCamera::Move()
{
	/*if (m_player->IsEnableMove() == false)
	{
		return;
	}
	}*/
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
	//���C
	m_moveSpeed.x -= m_moveSpeed.x * 0.3f;
	m_moveSpeed.z -= m_moveSpeed.z * 0.3f;
	//2�x�N�g���Ԃ̋�����0.001�ȉ���������
	if (m_moveSpeed.Length() < 0.001f)
	{
		m_moveSpeed.x -= m_moveSpeed.x * 0.3f;
		m_moveSpeed.z -= m_moveSpeed.z * 0.3f;
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
	m_rotation = qRot;

	//�J�����̉�]�̏�����`�F�b�N����B
	//�����_���王�_�܂ł̃x�N�g���𐳋K������B
	Vector3 toPosDir = m_toCameraPos;
	toPosDir.Normalize();
	if (toPosDir.y < TARGET_UNDER) {
		//�J�����������������B
		m_toCameraPos = toCameraPosOld;
	}
	else if (toPosDir.y > TARGET_OVER) {
		//�J����������������B
		m_toCameraPos = toCameraPosOld;
	}

	//���_���v�Z����B
	m_target = m_position + m_toCameraPos;
	//���C���J�����ɒ����_�Ǝ��_��ݒ肷��B
	g_camera3D->SetTarget(m_target);
}

void GameCamera::ManageState()
{
	switch (m_moveState)
	{
		//�ҋ@�X�e�[�g
	case enMoveState_Idle:
		IdleState();
		break;
		//�����X�e�[�g
	case enMoveState_Walk:
		WalkState();
		break;
		//����X�e�[�g
	case enMoveState_Run:
		RunState();
		break;
		//���Ⴊ�݃X�e�[�g
	case enMoveState_Sneak:
		SneakState();
		break;
	case enMoveState_SneakIdle:
		SneakIdleState();
		break;
	default:
		break;
	}
}

void GameCamera::IdleState()
{
	TransitionState();
}

void GameCamera::WalkState()
{
	TransitionState();
}

void GameCamera::RunState()
{
	TransitionState();
}

void GameCamera::SneakState()
{
	TransitionState();
}

void GameCamera::SneakIdleState()
{
	TransitionState();
}

void GameCamera::TransitionState()
{

	//x��z�̈ړ����x����������(�X�e�B�b�N�̓��͂���������)�B
	if (fabsf(m_moveSpeed.x) >= 0.01f && fabsf(m_moveSpeed.z) >= 0.01f)
	{
		//B�{�^���������Ă���Ԃ͑���B
		if (g_pad[0]->IsPress(enButtonA))
		{
			//�����Ԃɂ���B
			m_moveState = enMoveState_Run;
		}
		//LB�{�^���������Ă���Ԃ��Ⴊ�ށB
		if (g_pad[0]->IsPress(enButtonLB2))
		{
			//���Ⴊ�ݏ�Ԃɂ���B
			m_moveState = enMoveState_Sneak;
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

void GameCamera::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}