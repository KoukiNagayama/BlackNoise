#include "stdafx.h"
#include "Enemy.h"
#include "Player.h"

namespace
{
	const float MOVESPEED = 1.4f * 210.0f; //���s���x
	const float RUNSPEED = 1.4f;			//���s���x
	const float	SEARCHAREA = 1.3f * 1000.0f; //���G�͈�
}

bool Enemy::Start()
{
	m_player = FindGO<Player>("player");

	m_pathRender.Init("Assets/modelData/enemy/EnemyPass001.tkl", [&](LevelObjectData& objData)
		{
			if (objData.ForwardMatchName(L"Pass") == true)
			{
				if (objData.EqualObjectName(L"Pass0") == true)
				{
					SetPath(objData.position, 0);
				}
				else if (objData.EqualObjectName(L"Pass1") == true)
				{
					SetPath(objData.position, 1);
				}
				else if (objData.EqualObjectName(L"Pass2") == true)
				{
					SetPath(objData.position, 2);
				}
				else if (objData.EqualObjectName(L"Pass3") == true)
				{
					SetPath(objData.position, 3);
				}
				else if (objData.EqualObjectName(L"Pass4") == true)
				{
					SetPath(objData.position, 4);
				}
				else if (objData.EqualObjectName(L"Pass5") == true)
				{
					SetPath(objData.position, 5);
				}
				else if (objData.EqualObjectName(L"Pass6") == true)
				{
					SetPath(objData.position, 6);
				}
				else if (objData.EqualObjectName(L"Pass7") == true)
				{
					SetPath(objData.position, 7);
				}

				return true;
			}
		});

	m_animationClips[enAnimation_Normal].Load("Assets/animData/walk.tka");
	m_animationClips[enAnimation_Normal].SetLoopFlag(true);

	m_animationClips[enAnimation_Move].Load("Assets/animData/run.tka");
	m_animationClips[enAnimation_Move].SetLoopFlag(true);

	m_animationClips[enAnimation_Caution].Load("Assets/animData/idle.tka");
	m_animationClips[enAnimation_Caution].SetLoopFlag(true);

	m_naviMesh.Init("Assets/modelData/enemy/stage2_mesh.tkn");

	m_enemyRender.Init("Assets/modelData/unityChan.tkm", m_animationClips, enAnimation_Num, true, enModelUpAxisY);

	m_position = m_pathList[0].position;

	m_pathPoint = m_pathList[1];
	m_goalPosition = m_pathPoint.position;
	m_enemyRender.SetPosition(m_position);

	m_characterController.Init(10.0f, 45.0f, m_position);

	SetRoute();

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

	Vector3 pathPosition = m_path.Move(m_position, 60.0f, m_isEnd);

	if (m_isEnd == false);
	{
		//�o�H��̈ړ����x
		m_moveSpeed = pathPosition - m_oldPosition;

		m_toGoal = m_goalPosition - m_position;
		m_toGoal.y = 0.0f;
	}
	//�ڕW�ɓ���
	if (m_toGoal.Length() < 10.0f)
	{
		if (m_pathPoint.no != m_maxPath)
		{
			m_pathPoint = m_pathList[m_pathPoint.no];
		}
		else
		{
			m_pathPoint = m_pathList[0];
		}

		m_goalPosition = m_pathPoint.position;

		m_isEnd = true;
		SetRoute();
	}

}

bool Enemy::SearchSound()
{
	m_targetPosition = m_player->GetPosition();

	//�G�l�~�[����v���C���[�����ւ̃x�N�g��
	Vector3 toPlayer = m_targetPosition - m_position;
	toPlayer.y = 0.0f;

	//�v���C���[�����G�͈͓�
	if (toPlayer.Length() < SEARCHAREA)
	{
		//�����o��
		if (g_pad[0]->IsTrigger(enButtonB))
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
		m_isEnd = false;
	}
}

void Enemy::SetNearestPath()
{
	//���݈ʒu����ŒZ�̃p�X�̈ʒu
	m_nearestPath = m_pathList[0];

	//�ŒZ�̃p�X�܂ł̋���
	float	min = (m_nearestPath.position - m_position).Length();

	for (int i = 1; i < m_maxPath; i++)
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
	m_goalPosition = m_nearestPath.position;
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
		m_isEnd = false;
	}
}

void Enemy::StateToReturn()
{
	//�A�ҏ�ԂɈڍs����܂ł̃J�E���g�_�E��
	m_lostTimer -= 0.5f * g_gameTime->GetFrameDeltaTime();

	//�x����Ԃ̂܂܈�莞�Ԃ��o��
	if (m_lostTimer <= 0.0f)
	{
		m_lostTimer = 0.0f;
		//�Ŋ��̃p�X������
		SetNearestPath();
		//�Ŋ��̃p�X�܂ł̌o�H��ݒ�
		SetRoute();

		//�A�ҏ�Ԃֈڍs
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

void Enemy::PlayAnimation()
{
	switch (m_state)
	{
	case enNormal:
		m_enemyRender.PlayAnimation(enAnimation_Normal);
		break;

	case enMove:
		m_enemyRender.PlayAnimation(enAnimation_Move);
		break;

	case enCaution:
		m_enemyRender.PlayAnimation(enAnimation_Caution);
		break;

	case enReturn:
		m_enemyRender.PlayAnimation(enAnimation_Normal);
		break;
	}
}

void Enemy::Update()
{
	Move();
	ManageState();
	Rotation();
	PlayAnimation();

	m_enemyRender.Update();
}

void Enemy::Render(RenderContext& rc)
{
	m_enemyRender.Draw(rc);
}