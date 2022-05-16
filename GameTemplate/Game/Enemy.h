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
		enNormal,  //徘徊
		enMove,	   //発見
		enCaution, //警戒
		enReturn,   //帰還
		enAttack,	//攻撃
		enScream	//咆哮
	};

private:
	struct PathPoint
	{
		Vector3 position = Vector3::Zero; //位置
		int		no = 0;		  //番号
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
	void ManageState();    //ステート管理
	void SwitchState();
	void StateToNormal();  //徘徊状態に移行
	void StateToMove();	   //発見状態に移行
	void StateToCaution(); //警戒状態に移行
	void StateToReturn();  //帰還状態に移行
	void StateToAttack();
	void StateToScream();
	void Move();		   //移動
	void SelectMove();     //エネミーの状態によってどう移動するかを決定する
	void MoveByRoute();	   //発見、帰還状態の経路探索
	void Rotation();
	bool SearchSound();    //索敵範囲内で鳴っている音を感知する
	bool SearchPlayer();
	void Update();
	void Render(RenderContext& rc);
	void SetPath(const Vector3& position, const int& no)//徘徊状態のパスを設定する
	{
		m_pathList.push_back({ position, no + 1 });
	}
	void SetPosition(const Vector3& position)
	{
		m_position = position;
		m_enemyRender.SetPosition(m_position);
	}
	void SetRoute(); //経路探索のルートを設定
	void SetNearestPath(); //最寄りのパスを設定

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
	int					   m_state = enNormal;					 //ステート
	ModelRender			   m_enemyRender;
	Vector3				   m_position = Vector3::Zero; //座標位置
	Quaternion			   m_rotation = Quaternion::Identity;    //回転
	Vector3				   m_moveSpeed = Vector3::Zero;			 //移動速度
	float				   m_lostTimer = 10.0f;					 //移動先でプレイヤーを探す時間
	const float			   m_defaultLostTimer = m_lostTimer;
	Vector3				   m_goalPosition;						 //発見時の目的地座標
	float				   m_moveState = 1.0f;					 //ステートによって移動速度を変更させる
	CharacterController    m_characterController;
	std::vector<PathPoint> m_pathList;							 //パス全体
	PathPoint			   m_pathPoint;							 //現在の目標パス
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
	PathPoint			   m_nearestPath;						 //最寄りのパス
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