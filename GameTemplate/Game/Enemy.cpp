#include "stdafx.h"
#include "Enemy.h"
#include "Player.h"
#include <time.h>
#include <stdlib.h>
namespace
{
	const float MOVESPEED = 1.4f * 70.0f; //���s���x
	const float RUNSPEED = 1.4f;			//���s���x
	const float	SEARCHAREA = 1.3f * 400.0f; //���G�͈�
}

bool Enemy::Start()
{
	m_enemyRender.Init("Assets/modelData/unityChan.tkm");

	m_characterController.Init(10.0f, 45.0f, m_position);

	m_naviMesh.Init("Assets/modelData/testStage/nvm/test.tkn");

	return true;
}

void Enemy::Rotation()
{
	if (fabsf(m_moveSpeed.x) < 0.1f && fabsf(m_moveSpeed.z) < 0.1f)
	{
		return;
	}

	float angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);

	m_rotation.SetRotationY(-angle);

	m_enemyRender.SetRotation(m_rotation);
}

void Enemy::Move()
{
	//�X�e�[�g�ɂ���Ăǂ��ړ����邩�����肷��
	SelectMove();

	m_moveSpeed.y = 0.0f;
	m_moveSpeed.Normalize();

	m_moveSpeed *= MOVESPEED;

	//�������
	if (m_state == enMove)
	{
		m_moveSpeed *= RUNSPEED;
	}

	m_position = m_characterController.Execute(m_moveSpeed, 1.0f / 60.0f);

	m_enemyRender.SetPosition(m_position);
}

void Enemy::SelectMove()
{
	switch (m_state)
	{
	case enNormal:  //�p�j���

		MoveByRoute();
		break;

	case enMove:	   //�������
		MoveByRoute(); //���̔����n�_�֌o�H�T��
		break;

	case enCaution:					 //�x�����
		m_moveSpeed = Vector3::Zero; //�ړ����x�̓[��
		break;

	case enReturn:
		MoveByRoute();
		break;
	}
}

//�o�H�T���ňړ�
void Enemy::MoveByRoute()
{
	m_oldPosition = m_position;

	Vector3 pathPosition = m_path.Move(m_position, 3.0f, m_isEnd);

	if (m_isEnd == false);
	{
		//�o�H��̈ړ����x
		m_moveSpeed = pathPosition - m_oldPosition;
	}

	m_toGoal = m_goalPosition - m_position;
	m_toGoal.y = 0.0f;

	//�ڕW�ɓ���
	if (m_toGoal.Length() < 10.0f)
	{
		SetNextPath();
		m_isEnd = true;
	}
}

void Enemy::SetNextPath()
{
	do
	{
		m_pathRan = rand() % m_pathList.size();
	} while (m_pathRan == m_pathPoint.no - 1);

	m_pathPoint = m_pathList[m_pathRan];

	m_goalPosition = m_pathPoint.position;

	SetRoute();
}

bool Enemy::SearchSound()
{
	//�G�l�~�[����v���C���[�����ւ̃x�N�g��
	Vector3 toPlayer = m_targetPosition - m_position;
	toPlayer.y = 0.0f;

	//�v���C���[�����G�͈͓�
	if (toPlayer.Length() < SEARCHAREA)
	{
		//�����o��
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			//�ڕW�n�_�ւ̃p�X��ݒ�
			m_goalPosition = m_targetPosition;
			SetRoute();

			//�v���C���[�𔭌�
			return true;
		}
	}
	else
	{
		return false;
	}
}

void Enemy::ManageState()
{
	//�v���C���[�𔭌�
	if (SearchSound() == true)
	{
		m_isEnd = false;

		//������ԂɈڍs
		StateToMove();

		return;
	}

	switch (m_state)
	{
		//�������
	case enMove:
		//�x����ԂɈڍs
		StateToCaution();
		break;

		//�x�����
	case enCaution:
		//�Ŋ��̃p�X������
		SetNearestPath();
		//�A�ҏ�ԂɈڍs
		StateToReturn();
		break;

		//�A�ҏ��
	case enReturn:
		//�p�j���
		StateToNormal();
		break;
	}

	//�x����ԂłȂ�
	if (m_state != enCaution)
	{
		//�T�����Ԃ����Z�b�g
		m_lostTimer = 5.0f;
	}
}

void Enemy::StateToNormal()
{
	//�ڕW�n�_�ɓ���
	if (m_isEnd == true)
	{
		//�p�j��ԂɈڍs
		m_state = enNormal;
	}
}

void Enemy::SetNearestPath()
{
	//���݈ʒu����ŒZ�̃p�X�̈ʒu
	m_nearestPath = m_pathList[0];

	//�ŒZ�̃p�X�܂ł̋���
	float	min = (m_nearestPath.position - m_position).Length();

	for (int i = 1; i < m_pathList.size(); i++)
	{
		PathPoint path = m_pathList[i];
		float	length = (path.position - m_position).Length();

		//�����_�ōŒZ�̃p�X���߂��p�X�������
		if (min > length)
		{
			//�ŒZ�̃p�X�̈ʒu�Ƌ������X�V
			min = length;
			m_nearestPath = path;
		}
	}

	//�ŏI�I�ɍŒZ�̃p�X��ڕW�ɐݒ�
	m_pathPoint = m_nearestPath;
}

void Enemy::StateToMove()
{
	//������ԂɈڍs
	m_state = enMove;
}

void Enemy::StateToCaution()
{
	//�ڕW�n�_�ɓ���
	if (m_isEnd == true)
	{
		//�x����ԂɈڍs
		m_state = enCaution;
	}
}

void Enemy::StateToReturn()
{
	//�A�ҏ�ԂɈڍs����܂ł̃J�E���g�_�E��
	m_lostTimer -= 0.5f * g_gameTime->GetFrameDeltaTime();

	//�x����Ԃ̂܂܈�莞�Ԃ��o��
	if (m_lostTimer <= 0.0f)
	{
		//�Ŋ��̃p�X�܂ł̌o�H��ݒ�
		m_goalPosition = m_nearestPath.position;
		SetRoute();

		//�p�j��Ԃֈڍs
		m_state = enReturn;
	}
}

//�o�H�T���̌o�H��ݒ�
void Enemy::SetRoute()
{
	m_pathFinding.Execute
	(
		m_path,
		m_naviMesh,
		m_position,
		m_goalPosition,
		nullptr,
		30.0f,
		45.0f
	);
}

void Enemy::Update()
{
	Move();
	ManageState();
	Rotation();

	m_enemyRender.Update();
}

void Enemy::Render(RenderContext& rc)
{
	m_enemyRender.Draw(rc);
}