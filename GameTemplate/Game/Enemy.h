#pragma once
#include "AI/PathFinding/PathFinding.h"
#include "AI/PathFinding/NaviMesh.h"
#include "AI/PathFinding/Path.h"
#include <LevelRender.h>
#include <sound/SoundSource.h>

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
		enAttack,	//�U��
		enScream	//���K
	};

private:
	struct PathPoint
	{
		Vector3 position = Vector3::Zero; //�ʒu
		int		no = 0;		  //�ԍ�
	};

	enum AnimationClips
	{
		enAnimation_Normal,
		enAnimation_Move,
		enAnimation_Caution,
		enAnimation_Attack,
		enAnimation_Scream,
		enAnimation_Num
	};

	enum Alert
	{
		enAlert_Safe,
		enAlert_Caution,
		enAlert_Warning,
		enAlert_Danger,
		enAlert_Num
	};

public:
	Enemy() {};
	~Enemy() {};
	bool Start();
	void InitEnemy();
	void ManageState();    //�X�e�[�g�Ǘ�
	void SwitchState();
	void StateToNormal();  //�p�j��ԂɈڍs
	void StateToMove();	   //������ԂɈڍs
	void StateToCaution(); //�x����ԂɈڍs
	void StateToReturn();  //�A�ҏ�ԂɈڍs
	void StateToAttack();
	void StateToScream();
	void Move();		   //�ړ�
	void SelectMove();     //�G�l�~�[�̏�Ԃɂ���Ăǂ��ړ����邩�����肷��
	void MoveByRoute();	   //�����A�A�ҏ�Ԃ̌o�H�T��
	void Rotation();
	bool SearchSound();    //���G�͈͓��Ŗ��Ă��鉹�����m����
	bool SearchPlayer();
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

	void SetEndPath(const int& end)
	{
		//m_endPath = end;
	}
	void PlayAnimation();
	void ChangeSpeed();
	void SetPathLevel();
	void SetAnimationClips();
	void InitSound(Vector3 target, bool button);
	void UpAlertLevel();
	void DownAlertLevel();
	bool FindPlayer();
	void SetSound();
	void ChangePath();
	void SearchNearestPath();

	const Vector3 GetPosition()
	{
		return m_position;
	}

private:
	int					   m_alertLevel = enAlert_Safe;
	int					   m_state = enNormal;					 //�X�e�[�g
	ModelRender			   m_enemyRender;
	Vector3				   m_position = Vector3::Zero; //���W�ʒu
	Quaternion			   m_rotation = Quaternion::Identity;    //��]
	Vector3				   m_moveSpeed = Vector3::Zero;			 //�ړ����x
	float				   m_lostTimer = 10.0f;					 //�ړ���Ńv���C���[��T������
	const float			   m_defaultLostTimer = m_lostTimer;
	Vector3				   m_goalPosition;						 //�������̖ړI�n���W
	float				   m_moveState = 1.0f;					 //�X�e�[�g�ɂ���Ĉړ����x��ύX������
	CharacterController    m_characterController;
	std::vector<PathPoint> m_pathList;							 //�p�X�S��
	PathPoint			   m_pathPoint;							 //���݂̖ڕW�p�X
	nsAI::Path			   m_path;
	nsAI::NaviMesh		   m_naviMesh;
	nsAI::PathFinding	   m_pathFinding;
	bool				   m_isEnd = false;
	bool				   m_alert = false;
	bool				   press = false;
	float				   m_disSpeed = 1.0f;
	float				   time = 0.0f;
	Player* m_player;
	Vector3				   m_forward = Vector3::AxisZ;
	Vector3				   m_oldPosition;
	Vector3				   m_toGoal;
	Vector3				   m_toPlayer = Vector3::Zero;
	Animation			   m_animation;
	PathPoint			   m_nearestPath;						 //�Ŋ��̃p�X
	LevelRender			   m_pathRender;
	SoundSource* m_heartSE;
	SoundSource* m_screamSE;
	AnimationClip		   m_animationClips[enAnimation_Num];
	SphereCollider		   m_sphereCollider;
	const int			   m_maxPath = 8;
	const float			   m_attackRange = 100.0f;
	float				   m_searchAngle = (Math::PI / 180.0f) * 75.0f;
	float				   m_screamTimer = 3.0f;


	FontRender			   m_level;
	Vector3 player;
	float toPlayerAngle;
};