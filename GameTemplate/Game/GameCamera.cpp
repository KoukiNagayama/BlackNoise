#include "stdafx.h"
#include "sound/SoundEngine.h"
#include "GameCamera.h"
#include "Player.h"
#include "InfoForEdge.h"
#include "Enemy2.h"

namespace
{
	const float CAMPOS_Y = 250.0f;			//���_�̍���
	const float TOCAMPOS_Z = -77.0f;		//�����_�܂ł�Z���W
	const float DISTANCE = 100.0f;			//�v���C���[�Ƃ̋���	
	const float MOVESPEED = 500.0f;			//�����̈ړ����x
	const float MOVE_RUN = 1.22f;			//���莞�ɂ������Z���邩
	const float MOVE_SNEAK = 0.3f;			//���Ⴊ�ݎ��ɂ������Z���邩
	const float TARGET_UNDER = -0.7f;		//�J�����̉���
	const float TARGET_OVER = 0.35f;		//�J�����̏��
	const float EDGE_FADE_IN_DELTA_VALUE = 0.05f;	// �G�b�W���t�F�[�h�C������Ƃ��̕ψʗ�
	const float EDGE_FADE_OUT_DELTA_VALUE = 0.05f;	// �G�b�W���t�F�[�h�A�E�g����Ƃ��̕ψʗ�
	const float RATE_BY_TIME_MAX_VALUE = 1.00f;		// ���Ԃɂ��e�����̍ő�l
	const float RATE_BY_TIME_MIN_VALUE = 0.00f;		// ���Ԃɂ��e�����̍ŏ��l
	const float SOUND_RANGE = 180.0f;				//�e���͈�
	const float VOLUME = 0.7f;						//�{�����[��
	const float FRICTION = 0.5f;					//���C
	const float VIEWPOINT_UP = 350.0f;				//�G�̊�̍���
}

GameCamera::GameCamera()
{
}
GameCamera::~GameCamera()
{
	g_infoForEdge.SetIsSound(6, 0);
	g_infoForEdge.SetRate(6, 0.00f);
	DeleteGO(m_sound);
}
bool GameCamera::Start()
{
	//�����_���王�_�܂ł̃x�N�g����ݒ�B
	m_toCameraPos.Set(0.0f, 0.0f, TOCAMPOS_Z);

	//�v���C���[�̃C���X�^���X��T���B
	m_player = FindGO<Player>("player");
	//�G�l�~�[
	m_enemy = FindGO<Enemy2>("enemy");

	//m_position = Vector3::Zero;
	m_modelRender.Init("Assets/modelData/human/playerbox.tkm");
	m_modelRender.SetPosition(m_position);

	//�J�����̃j�A�N���b�v�ƃt�@�[�N���b�v��ݒ肷��B
	g_camera3D->SetNear(3.0f);
	g_camera3D->SetFar(10000.0f);

	//�L�����R��������������B
	m_charaCon.Init(50.0f, 170.0f, m_position);

	//�T�E���h��o�^�B
	g_soundEngine->ResistWaveFileBank(11, "Assets/sound/human/walk.wav");
	//�T�E���h��NewGO
	m_sound = NewGO<SoundSource>(11);
	m_sound->Init(11);
	m_sound->SetVolume(VOLUME);

	g_infoForEdge.InitForSound(6, m_position, 200.0f, 0, m_rateByTime);

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
	//�e�����𒲂ׂ�
	CheckRate();

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
	////x,z�̈ړ����x��0�ɂ���B
	//m_moveSpeed.x = 0.0f;
	//m_moveSpeed.z = 0.0f;


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

	//���C
	m_moveSpeed.x -= m_moveSpeed.x * FRICTION;
	m_moveSpeed.z -= m_moveSpeed.z * FRICTION;
	//2�x�N�g���Ԃ̋�����0.001�ȉ���������
	if (m_moveSpeed.Length() < 0.001f)
	{
		m_moveSpeed.x -= m_moveSpeed.x * FRICTION;
		m_moveSpeed.z -= m_moveSpeed.z * FRICTION;
	}
	
	//�L�����R�����g���č��W���ړ�������B
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	//���_�̍������グ��B
	m_position.y = CAMPOS_Y;
	//�J�����̍��W��ݒ肷��B
	g_camera3D->SetPosition(m_position);
}
//�����_�̏���
void GameCamera::ViewPoint()
{
	//�J�������X�V�B
	//�����_���v�Z����B
	//m_target = m_position;
	//�v���C���̑������炿����Ə�𒍎��_�Ƃ���B
	//m_target += g_camera3D->GetForward() * 50.0f;

	Vector3 toCameraPosOld = m_toCameraPos;
	//�p�b�h�̓��͂��g���ăJ�������񂷁B
	float x = g_pad[0]->GetRStickXF() * 1.5f;
	float y = g_pad[0]->GetRStickYF() * 1.5f;



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

	//�v���C���[���U�����󂯂���
	/*if (m_player->IsEnableMove() == false) {
		m_target = m_enemy->GetPosition();
		m_target.y += VIEWPOINT_UP;
	}*/
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
	default:
		break;
	}
}

void GameCamera::IdleState()
{
	m_sound->Pause();
	m_isSound = false;
	TransitionState();
}

void GameCamera::WalkState()
{
	m_sound->Play(true);
	m_isSound = true;
	TransitionState();
}

void GameCamera::TransitionState()
{

	//x��z�̈ړ����x����������(�X�e�B�b�N�̓��͂���������)�B
	if (fabsf(m_moveSpeed.x) >= 0.01f && fabsf(m_moveSpeed.z) >= 0.01f)
	{
		//������Ԃɂ���B
		m_moveState = enMoveState_Walk;
		return;
	}
	m_moveState = enMoveState_Idle;
}

void GameCamera::CheckRate()
{
	int check1;
	if (m_sound != nullptr) {
		if (m_moveState == enMoveState_Walk)
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
		g_infoForEdge.SetInfoForSound(6, GetYaxisZeroPosition(), SOUND_RANGE, check1, m_rateByTime);

	}
}

void GameCamera::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}