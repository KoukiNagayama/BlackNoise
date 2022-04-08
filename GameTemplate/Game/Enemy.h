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
		enNormal,  //徘徊
		enMove,	   //発見
		enCaution, //警戒
		enReturn,   //帰還
	};

private:
	struct PathPoint
	{
		Vector3 position = Vector3::Zero; //位置
		int		no = 0;		  //番号
	};

public:
	Enemy() {};
	~Enemy() {};
	bool Start();
	void ManageState();    //ステート管理
	void StateToNormal();  //徘徊状態に移行
	void StateToMove();	   //発見状態に移行
	void StateToCaution(); //警戒状態に移行
	void StateToReturn();  //帰還状態に移行
	void Move();		   //移動
	void SelectMove();     //エネミーの状態によってどう移動するかを決定する
	void MoveByRoute();	   //発見、帰還状態の経路探索
	void Rotation();
	bool SearchSound();    //索敵範囲内で鳴っている音を感知する
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
	void SetTarget(const Vector3& position)
	{
		m_targetPosition = position;
		m_targetPosition.y = 0.0f;
	}
	void SetNextPath();


private:
	ModelRender			   m_enemyRender;
	Vector3				   m_position = Vector3::Zero; //座標位置
	Quaternion			   m_rotation = Quaternion::Identity;    //回転
	int					   m_state = enNormal;					 //ステート
	Vector3				   m_moveSpeed = Vector3::Zero;			 //移動速度
	float				   m_lostTimer = 5.0f;					 //移動先でプレイヤーを探す時間
	Vector3				   m_goalPosition;						 //発見時の目的地座標
	float				   m_moveState = 1.0f;					 //ステートによって移動速度を変更させる
	CharacterController    m_characterController;
	std::vector<PathPoint> m_pathList;							 //パス全体
	PathPoint			   m_pathPoint;							 //現在の目標パス
	nsAI::PathFinding	   m_pathFinding;
	nsAI::NaviMesh		   m_naviMesh;
	nsAI::Path			   m_path;
	bool				   m_isEnd = false;
	Vector3				   m_toGoal;
	PathPoint			   m_nearestPath;						 //最寄りのパス
	Vector3				   m_oldPosition;
	Vector3				   m_targetPosition;
	int					   m_pathRan = 0;
};