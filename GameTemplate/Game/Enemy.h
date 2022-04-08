#pragma once
#include "AI/PathFinding/PathFinding.h"
#include "AI/PathFinding/NaviMesh.h"
#include "AI/PathFinding/Path.h"

class Player;
class GameCamera;

class Enemy : public IGameObject
{
public:
	enum enState
	{
		enNormal,  //�p�j
		enMove,	   //����
		enCaution, //�x��
		enReturn,   //�A��
	};

private:
	struct PathPoint
	{
		Vector3 position = Vector3::Zero; //�ʒu
		int		no = 0;		  //�ԍ�
	};

public:
	Enemy() {};
	~Enemy() {};
	bool Start();
	void ManageState();    //�X�e�[�g�Ǘ�
	void StateToNormal();  //�p�j��ԂɈڍs
	void StateToMove();	   //������ԂɈڍs
	void StateToCaution(); //�x����ԂɈڍs
	void StateToReturn();  //�A�ҏ�ԂɈڍs
	void Move();		   //�ړ�
	void SelectMove();     //�G�l�~�[�̏�Ԃɂ���Ăǂ��ړ����邩�����肷��
	void MoveByRoute();	   //�����A�A�ҏ�Ԃ̌o�H�T��
	void Rotation();
	bool SearchSound();    //���G�͈͓��Ŗ��Ă��鉹�����m����
	void Update();
	void Render(RenderContext& rc);
	void SetPath(const Vector3& position, const int& no)//�p�j��Ԃ̃p�X��ݒ肷��
	{
		m_pathList.push_back({ position, no + 1 });
	}
	void SetPosition(const Vector3& position)
	{
		m_position = position;
		m_enemyRender.SetPosition(m_position);
	}
	void SetRoute(); //�o�H�T���̃��[�g��ݒ�
	void SetNearestPath(); //�Ŋ��̃p�X��ݒ�
	void SetTarget(const Vector3& position)
	{
		m_targetPosition = position;
		m_targetPosition.y = 0.0f;
	}
	void SetNextPath();


private:
	ModelRender			   m_enemyRender;
	Vector3				   m_position = Vector3::Zero; //���W�ʒu
	Quaternion			   m_rotation = Quaternion::Identity;    //��]
	int					   m_state = enNormal;					 //�X�e�[�g
	Vector3				   m_moveSpeed = Vector3::Zero;			 //�ړ����x
	float				   m_lostTimer = 5.0f;					 //�ړ���Ńv���C���[��T������
	Vector3				   m_goalPosition;						 //�������̖ړI�n���W
	float				   m_moveState = 1.0f;					 //�X�e�[�g�ɂ���Ĉړ����x��ύX������
	CharacterController    m_characterController;
	std::vector<PathPoint> m_pathList;							 //�p�X�S��
	PathPoint			   m_pathPoint;							 //���݂̖ڕW�p�X
	nsAI::PathFinding	   m_pathFinding;
	nsAI::NaviMesh		   m_naviMesh;
	nsAI::Path			   m_path;
	bool				   m_isEnd = false;
	Vector3				   m_toGoal;
	PathPoint			   m_nearestPath;						 //�Ŋ��̃p�X
	Vector3				   m_oldPosition;
	Vector3				   m_targetPosition;
	int					   m_pathRan = 0;
};