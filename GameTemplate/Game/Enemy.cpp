#include "stdafx.h"
#include "Enemy.h"
#include "Player.h"

namespace
{
	const float MOVESPEED = 1.4f * 210.0f; //歩行速度
	const float RUNSPEED = 1.4f;			//走行速度
	const float	SEARCHAREA = 1.3f * 1000.0f; //索敵範囲
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
	//ステートによってどう移動するかを決定する
	SelectMove();

	m_moveSpeed.y = 0.0f;
	m_moveSpeed.Normalize();

	m_moveSpeed *= MOVESPEED;

	//発見状態
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
	case enNormal:  //徘徊状態
		MoveByRoute();
		break;

	case enMove:	   //発見状態
		MoveByRoute(); //音の発生地点へ経路探索
		break;

	case enCaution:					 //警戒状態
		m_moveSpeed = Vector3::Zero; //移動速度はゼロ
		break;

	case enReturn:
		MoveByRoute();
		break;
	}
}

//経路探索で移動
void Enemy::MoveByRoute()
{
	m_oldPosition = m_position;

	Vector3 pathPosition = m_path.Move(m_position, 60.0f, m_isEnd);

	if (m_isEnd == false);
	{
		//経路上の移動速度
		m_moveSpeed = pathPosition - m_oldPosition;

		m_toGoal = m_goalPosition - m_position;
		m_toGoal.y = 0.0f;
	}
	//目標に到着
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

	//エネミーからプレイヤー方向へのベクトル
	Vector3 toPlayer = m_targetPosition - m_position;
	toPlayer.y = 0.0f;

	//プレイヤーが索敵範囲内
	if (toPlayer.Length() < SEARCHAREA)
	{
		//音を出す
		if (g_pad[0]->IsTrigger(enButtonB))
		{
			//目標地点へのパスを設定
			m_goalPosition = m_targetPosition;
			SetRoute();

			//プレイヤーを発見
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
	//プレイヤーを発見
	if (SearchSound() == true)
	{
		m_isEnd = false;

		//発見状態に移行
		StateToMove();

		return;
	}

	switch (m_state)
	{
		//発見状態
	case enMove:
		//警戒状態に移行
		StateToCaution();
		break;

		//警戒状態
	case enCaution:
		//帰還状態に移行
		StateToReturn();
		break;

		//帰還状態
	case enReturn:
		//徘徊状態
		StateToNormal();
		break;
	}

	//警戒状態でない
	if (m_state != enCaution)
	{
		//探索時間をリセット
		m_lostTimer = 5.0f;
	}
}

void Enemy::StateToNormal()
{
	//目標地点に到着
	if (m_isEnd == true)
	{
		//徘徊状態に移行
		m_state = enNormal;
		m_isEnd = false;
	}
}

void Enemy::SetNearestPath()
{
	//現在位置から最短のパスの位置
	m_nearestPath = m_pathList[0];

	//最短のパスまでの距離
	float	min = (m_nearestPath.position - m_position).Length();

	for (int i = 1; i < m_maxPath; i++)
	{
		PathPoint path = m_pathList[i];
		float	length = (path.position - m_position).Length();

		//現時点で最短のパスより近いパスがあれば
		if (min > length)
		{
			//最短のパスの位置と距離を更新
			min = length;
			m_nearestPath = path;
		}
	}

	//最終的に最短のパスを目標に設定
	m_pathPoint = m_nearestPath;
	m_goalPosition = m_nearestPath.position;
}

void Enemy::StateToMove()
{
	//発見状態に移行
	m_state = enMove;
}

void Enemy::StateToCaution()
{
	//目標地点に到着
	if (m_isEnd == true)
	{
		//警戒状態に移行
		m_state = enCaution;
		m_isEnd = false;
	}
}

void Enemy::StateToReturn()
{
	//帰還状態に移行するまでのカウントダウン
	m_lostTimer -= 0.5f * g_gameTime->GetFrameDeltaTime();

	//警戒状態のまま一定時間が経過
	if (m_lostTimer <= 0.0f)
	{
		m_lostTimer = 0.0f;
		//最寄りのパスを検索
		SetNearestPath();
		//最寄りのパスまでの経路を設定
		SetRoute();

		//帰還状態へ移行
		m_state = enReturn;
	}
}

//経路探索の経路を設定
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