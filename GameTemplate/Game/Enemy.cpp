#include "stdafx.h"
#include "Enemy.h"
#include "Player.h"
#include <sound/SoundEngine.h>

namespace
{
	const float MOVESPEED = 1.4f * 210.0f; //歩行速度
	const float RUNSPEED = 1.4f;			//走行速度
	const float	SEARCHAREA = 1.3f * 2000.0f; //索敵範囲
}

struct SweepResultWall : public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;

	virtual btScalar addSingleResult(
		btCollisionWorld::LocalConvexResult& convexResult,
		bool normalInWorldSpace)
	{
		if (convexResult.m_hitCollisionObject->getUserIndex() != enCollisionAttr_Wall)
		{
			return 0.0f;
		}

		isHit = true;
		return 0.0f;
	}
};

bool Enemy::Start()
{
	m_player = FindGO<Player>("player");

	SetPathLevel();

	SetSound();

	SetAnimationClips();

	InitEnemy();

	SetRoute();

	return true;
}

void Enemy::InitEnemy()
{
	m_sphereCollider.Create(1.0f);

	m_naviMesh.Init("Assets/modelData/enemy/stage2_mesh.tkn");

	m_enemyRender.Init("Assets/modelData/enemy/enemy.tkm", m_animationClips, enAnimation_Num, true);
	m_enemyRender.SetScale(Vector3::One * 5.0f);

	m_position = m_pathList[0].position;

	m_pathPoint = m_pathList[1];

	m_goalPosition = m_pathPoint.position;
	m_enemyRender.SetPosition(m_position);

	m_characterController.Init(10.0f, 45.0f, m_position);
}

void Enemy::SetSound()
{
	g_soundEngine->ResistWaveFileBank(4, "Assets/sound/enemy/heart_beat/heart_beat_caution.wav");
	g_soundEngine->ResistWaveFileBank(5, "Assets/sound/enemy/heart_beat/Heart_Beat/heart_beat_warning.wav");
	g_soundEngine->ResistWaveFileBank(6, "Assets/sound/enemy/heart_beat/Heart_Beat/heart_beat_danger.wav");
	g_soundEngine->ResistWaveFileBank(7, "Assets/sound/enemy/scream/scream.wav");
}

void Enemy::SetAnimationClips()
{
	m_animationClips[enAnimation_Normal].Load("Assets/animData/enemy/walk.tka");
	m_animationClips[enAnimation_Normal].SetLoopFlag(true);

	m_animationClips[enAnimation_Move].Load("Assets/animData/enemy/run.tka");
	m_animationClips[enAnimation_Move].SetLoopFlag(true);

	m_animationClips[enAnimation_Caution].Load("Assets/animData/enemy/lookaround.tka");
	m_animationClips[enAnimation_Caution].SetLoopFlag(true);

	m_animationClips[enAnimation_Attack].Load("Assets/animData/enemy/attack.tka");
	m_animationClips[enAnimation_Attack].SetLoopFlag(false);

	m_animationClips[enAnimation_Scream].Load("Assets/animData/enemy/lookaround.tka");
	m_animationClips[enAnimation_Scream].SetLoopFlag(false);
}

void Enemy::SetPathLevel()
{
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
}

void Enemy::Rotation()
{
	if (fabsf(m_moveSpeed.x) < 0.1f && fabsf(m_moveSpeed.z) < 0.1f)
	{
		/*
		if (m_state == enCaution)
		{
			if (angle > 90.0f || angle < -90.0f)
			{
				if (angle > 90.0f)
				{
					angle = 90.0f;
				}
				else if (angle < -90.0f)
				{
					angle = -90.0f;
				}

				change *= -1.0f;
			}

			angle += change;// *g_gameTime->GetFrameDeltaTime();

			m_rotation.AddRotationDegY(change);
			m_enemyRender.SetRotation(m_rotation);
		}
		*/
		return;
	}

	float angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);
	if (m_state == enAttack)
	{
		angle = atan2(-m_toPlayer.x, m_toPlayer.z);
	}

	m_rotation.SetRotationY(-angle);
	m_enemyRender.SetRotation(m_rotation);
	m_rotation.Apply(m_forward);

}

void Enemy::Move()
{
	m_toPlayer = m_player->GetPosition();
	m_toPlayer -= m_position;
	m_toPlayer.y = 0.0f;

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
	else if (m_state == enNormal)
	{
		ChangeSpeed();
	}
	m_position = m_characterController.Execute(m_moveSpeed, 1.0f / 60.0f);

	m_enemyRender.SetPosition(m_position);
}

void Enemy::ChangeSpeed()
{
	float lower = 0.3f;

	if (m_toGoal.Length() < 100.0f)
	{
		m_disSpeed -= 0.05f;

		if (m_disSpeed < lower)
		{
			m_disSpeed = lower;
		}
	}
	else
	{
		m_disSpeed += 0.05f;

		if (m_disSpeed > 1.0f)
		{
			m_disSpeed = 1.0f;
		}
	}

	m_moveSpeed *= m_disSpeed;
}

void Enemy::SelectMove()
{
	switch (m_state)
	{
	case enNormal:  //徘徊状態
		MoveByRoute();
		break;

	case enMove:	   //発見状態
		if (m_lostTimer > 0.0f)
		{
			SetRoute();
		}

		MoveByRoute(); //音の発生地点へ経路探索
		break;

	case enCaution:					 //警戒状態
		m_moveSpeed = Vector3::Zero; //移動速度はゼロ
		break;

	case enReturn:
		MoveByRoute();
		break;

	case enAttack:
		m_moveSpeed = Vector3::Zero;
		break;

	case enScream:
		m_moveSpeed = Vector3::Zero;
		break;
	}
}

//経路探索で移動
void Enemy::MoveByRoute()
{
	m_oldPosition = m_position;

	Vector3 pathPosition = m_path.Move(m_position, 60.0f, m_isEnd);

	if (m_isEnd == false)
	{
		//経路上の移動速度
		m_moveSpeed = pathPosition - m_oldPosition;

		m_toGoal = m_goalPosition - m_position;
		m_toGoal.y = 0.0f;
	}

	ChangePath();

	//m_isEnd = true;
}

void Enemy::ChangePath()
{
	//目標に到着
	if (m_toGoal.Length() < 1.0f)
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

		SetRoute();

		m_isEnd = false;
	}
}

bool Enemy::SearchSound()
{
	if (press == true)
	{
		if (m_state != enScream)
		{
			press = false;
		}
		/*	time += g_gameTime->GetFrameDeltaTime();

			if (time > 3.0f)
			{
				time = 0.0f;

				press = false;
			}*/
	}

	InitSound(m_player->GetPosition(), press);

	//音を出す
	if (g_pad[0]->IsTrigger(enButtonB))
	{
		//プレイヤーが索敵範囲内
		if (m_toPlayer.Length() < SEARCHAREA)
		{
			if (press == false)
			{
				//m_goalPosition = m_player->GetPosition();

				press = true;

				UpAlertLevel();
			}

			return true;
		}
		else
		{
			press = true;

			return false;
		}
	}
}

bool Enemy::SearchPlayer()
{
	player = m_toPlayer;
	player.Normalize();

	//m_forward.Normalize();

	toPlayerAngle = Dot(player, m_forward);

	//toPlayerAngle = player.x * m_forward.x;
	//toPlayerAngle += player.y * m_forward.y;
	//toPlayerAngle += player.z * m_forward.z;

	////目標地点へのパスを設定
	//m_goalPosition = m_toPlayer + m_position;
	//SetRoute();

	if (fabsf(acosf(toPlayerAngle)) <= m_searchAngle)
	{
		return true;

		/*	if (FindPlayer() == true)
			{
				return true;
			}
			else
			{
				return false;
			}*/
	}
	else
	{
		return false;
	}
}

bool Enemy::FindPlayer()
{
	Vector3 player = m_player->GetPosition();

	btTransform start, end;
	start.setIdentity();
	end.setIdentity();

	const float vertical = 70.0f;

	start.setOrigin(btVector3(m_position.x, m_position.y + vertical, m_position.z));
	end.setOrigin(btVector3(player.x, player.y + vertical, player.z));

	SweepResultWall callback;

	PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);

	if (callback.isHit == true)
	{
		return false;//見つけていない
	}
	else
	{
		return true;//見つけた
	}
}
void Enemy::InitSound(Vector3 target, bool button)
{
	const float alertRange = 1.5f;
	Vector3 toTarget = target - m_position;

	if (target.Length() < SEARCHAREA * alertRange)
	{
		if (m_alert == false)
		{
			if (button == true)
			{
				m_heartSE = NewGO<SoundSource>(0, "heart");
				m_heartSE->Init(m_alertLevel + 3);
				m_heartSE->SetVolume(1.0f);
				m_heartSE->Play(true);

				m_alert = true;
			}
		}
		else
		{
			if (button == true)
			{
				DeleteGO(m_heartSE);

				m_heartSE = NewGO<SoundSource>(0, "heart");
				m_heartSE->Init(m_alertLevel + 3);
				m_heartSE->Play(true);
			}
		}
		/*
		else
		{
			if (button == true)
			{
				DeleteGO(m_heartSE);
				m_alert = false;
			}
		}
		*/
	}
	else
	{
		if (m_alert == true)
		{
			DeleteGO(m_heartSE);
			m_alert = false;
		}
	}
}

void Enemy::ManageState()
{
	if (m_state == enAttack)
	{
		if (m_enemyRender.IsPlayingAnimation() == true)
		{
			return;
		}
	}

	if (m_alertLevel == enAlert_Danger)
	{
		if (m_toPlayer.Length() <= SEARCHAREA)
		{
			StateToMove();
		}
	}

	SwitchState();
}

void Enemy::SwitchState()
{
	if (m_state != enMove)
	{
		m_searchAngle = (Math::PI / 180.0f) * 30.0f;
	}

	switch (m_state)
	{
	case enNormal:
		if (m_alertLevel > enAlert_Safe)
		{
			DownAlertLevel();
		}

		if (SearchSound() == true)
		{
			if (m_alertLevel < enAlert_Danger)
			{
				StateToScream();
			}
		}

		break;

		//発見状態
	case enMove:

		m_searchAngle = (Math::PI / 180.0f) * 30.0f;

		if (m_toPlayer.Length() <= m_attackRange)
		{
			StateToAttack();
		}

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

	case enAttack:
		StateToCaution();
		break;

	case enScream:
		StateToNormal();
		break;
	}
}

void Enemy::StateToNormal()
{
	if (m_state == enScream)
	{
		if (m_screamSE->IsPlaying() == false)
		{
			//SetRoute();
			DeleteGO(m_screamSE);

			m_state = enNormal;
		}
	}

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

	SearchNearestPath();

	//最終的に最短のパスを目標に設定
	m_pathPoint = m_nearestPath;
	m_goalPosition = m_nearestPath.position;
}

void Enemy::SearchNearestPath()
{
	Vector3 toPath = m_nearestPath.position - m_position;
	//最短のパスまでの距離
	float	min = toPath.Length();
	toPath.Normalize();
	float nearDot = Dot(m_forward, toPath);

	for (int i = 1; i < m_maxPath; i++)
	{
		PathPoint path = m_pathList[i];
		toPath = m_pathList[i].position - m_position;
		float	length = toPath.Length();

		//現時点で最短のパスより近いパスがあれば
		if (min > length)
		{
			float dot = Dot(toPath, m_forward);

			if (fabsf(acosf(dot)) > fabsf(acosf(nearDot)))
			{
				//最短のパスの位置と距離を更新
				min = length;
				m_nearestPath = path;
				nearDot = dot;
			}
		}
	}
}

void Enemy::StateToMove()
{
	//発見状態に移行
	m_isEnd = false;

	m_state = enMove;
}

void Enemy::StateToCaution()
{
	if (m_state == enAttack)
	{
		if (m_enemyRender.IsPlayingAnimation() == false)
		{
			m_state = enCaution;

			return;
		}
	}

	if (m_toPlayer.Length() > SEARCHAREA * 0.5f)
	{
		if (SearchPlayer() == false)
		{
			m_lostTimer -= 1.0f * g_gameTime->GetFrameDeltaTime();

			if (m_lostTimer < 0.0f)
			{
				if (m_isEnd == true)
				{
					m_isEnd = false;

					m_lostTimer = m_defaultLostTimer;

					m_state = enCaution;
				}
			}
		}
	}

	//目標地点に到着
	//if (m_isEnd == true)
	//{
	//	//警戒状態に移行
	//	m_isEnd = false;
	//	m_state = enCaution;
	//}
}

void Enemy::StateToReturn()
{
	//帰還状態に移行するまでのカウントダウン
	m_lostTimer -= 2.0f * g_gameTime->GetFrameDeltaTime();

	//警戒状態のまま一定時間が経過
	if (m_lostTimer <= 0.0f)
	{
		m_lostTimer = m_defaultLostTimer;

		DownAlertLevel();
		//最寄りのパスを検索
		SetNearestPath();
		//最寄りのパスまでの経路を設定
		SetRoute();

		//帰還状態へ移行
		m_state = enReturn;
	}
}

void Enemy::StateToAttack()
{

	if (SearchPlayer() == true)
	{
		m_state = enAttack;
	}

	/*const float attackRange = 300.0f;

	if (m_alertLevel == enAlert_Danger)
	{
		if (SearchPlayer() == true)
		{
			if (m_toPlayer.Length() <= attackRange)
			{
				m_state = enAttack;
			}
		}
	}*/

}

void Enemy::StateToScream()
{
	m_screamSE = NewGO<SoundSource>(0, "scream");
	m_screamSE->Init(7);
	m_screamSE->SetVolume(0.5f);
	m_screamSE->Play(false);

	m_state = enScream;
}

//経路探索の経路を設定
void Enemy::SetRoute()
{
	if (m_state == enMove)
	{
		if (m_lostTimer > 0.0f)//m_toPlayer.Length() > m_attackRange||m_lostTimer>0.0f)
		{
			m_goalPosition = m_player->GetPosition();
		}
	}

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

	case enAttack:
		m_enemyRender.PlayAnimation(enAnimation_Attack);
		break;

	case enScream:
		m_enemyRender.PlayAnimation(enAnimation_Scream);
		break;
	}
}

void Enemy::UpAlertLevel()
{
	//if (m_alertLevel < enAlert_Danger)
	//{
	//	m_alertLevel++;
	//}

	if (SearchPlayer() == true)
	{
		m_alertLevel = enAlert_Danger;
	}
	else
	{
		if (m_alertLevel < enAlert_Danger)
		{
			m_alertLevel++;
		}
	}
}

void Enemy::DownAlertLevel()
{
	if (m_state != enNormal)
	{
		if (m_state != enScream)
		{
			if (m_alertLevel > enAlert_Safe)
			{
				m_alertLevel--;
			}
		}
	}
	else
	{
		if (m_alertLevel > enAlert_Safe)
		{
			m_lostTimer -= 1.0f * g_gameTime->GetFrameDeltaTime();

			if (m_lostTimer < 0.0f)
			{
				m_lostTimer = m_defaultLostTimer;

				m_alertLevel--;
			}
		}
	}
}

void Enemy::Update()
{
	Move();
	ManageState();
	Rotation();
	PlayAnimation();

	m_enemyRender.Update();

	wchar_t wcsbuf[256];
	swprintf_s(wcsbuf, 256, L"alert:%d", m_alertLevel);
	m_level.SetText(wcsbuf);
	m_level.SetPosition({ 500.0f,300.0f,0.0f });
}

void Enemy::Render(RenderContext& rc)
{
	m_enemyRender.Draw(rc);

	m_level.Draw(rc);
}