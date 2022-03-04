#include "stdafx.h"
#include "Player.h"

namespace
{
	const float MOVESPEED = 70.0f;
	const float MOVE_RUN = 1.4f;
	const float MOVE_SNEAK = 0.58f;
}

bool Player::Start()
{
	m_modelRender.Init("Assets/modelData/human/jackie.tkm");
	m_modelRender.SetPosition(m_position);

	//キャラコンを初期化する。
	m_charaCon.Init(10.0f, 45.0f, m_position);

	return true;
}

Player::~Player()
{

}

void Player::Update()
{
	//移動処理。
	Move();
	//回転処理。
	Rotation();
	//ステート遷移処理。
	ManageState();	

	m_modelRender.Update();
}

void Player::Move()
{
	if (IsEnableMove() == false)
	{
		return;
	}
	//x,zの移動速度を0にする。
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;

	//左スティックの入力量を計算
	Vector3 stickL;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.y = g_pad[0]->GetLStickYF();

	//カメラの前方向と右方向のベクトルを取得。
	Vector3 forward = g_camera3D->GetForward();
	Vector3 right = g_camera3D->GetRight();

	//Y方向には移動させない
	forward.y = 0.0f;
	right.y = 0.0f;

	//移動速度にスティックの入力量を加算する。
	m_moveSpeed += right * stickL.x * MOVESPEED;
	m_moveSpeed += forward * stickL.y * MOVESPEED;

	//走りステートなら速度を1.3倍にする。
	if (m_playerState == enPlayerState_Run)
	{
		m_moveSpeed.x *= MOVE_RUN;
		m_moveSpeed.z *= MOVE_RUN;
	}
	//しゃがみステートなら速度を0.7倍にする。
	if (m_playerState == enPlayerState_Sneak)
	{
		m_moveSpeed.x *= MOVE_SNEAK;
		m_moveSpeed.z *= MOVE_SNEAK;
	}
	//地面についていたら
	if (m_charaCon.IsOnGround())
	{
		//重力をなくす。
		m_moveSpeed.y = 0.0f;
	}
	//キャラコンを使って座標を移動させる。
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	//座標を設定する。
	m_modelRender.SetPosition(m_position);
}

void Player::Rotation()
{

	//回転角度を計算する。
	float angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);
	m_rotation.SetRotationY(-angle);

	//回転を設定する。
	m_modelRender.SetRotation(m_rotation);

	//プレイヤーの正面ベクトルを計算する。
	m_rotation.Apply(m_forward);
}

void Player::TransitionState()
{
	//HPが0になったら
	if (m_hp == 0)
	{
		m_playerState = enPlayerState_Down;
		return;
	}
	//xかzの移動速度があったら(スティックの入力があったら)。
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		//Bボタンを押している間は走る。
		if (g_pad[0]->IsPress(enButtonB))
		{
			m_playerState = enPlayerState_Run;
			return;
		}
		//LBボタンを押している間しゃがむ。
		if (g_pad[0]->IsPress(enButtonLB2))
		{
			m_playerState = enPlayerState_Sneak;
			return;
		}
		m_playerState = enPlayerState_Walk;
	}
	else
	{
		//止まる前がしゃがみ状態だったら
		if (m_playerState = enPlayerState_SneakIdle)
		{
			m_playerState = enPlayerState_SneakIdle;
			return;
		}
		m_playerState = enPlayerState_Idle;
	}
}

void Player::ManageState()
{
	switch (m_playerState)
	{
		//待機ステート
	case enPlayerState_Idle:
		IdleState();
		break;
		//歩きステート
	case enPlayerState_Walk:
		WalkState();
		break;
		//走りステート
	case enPlayerState_Run:
		RunState();
		break;
		//しゃがみ状態
	case enPlayerState_Sneak:
		SneakState();
		break;
		//しゃがみ待機状態
	case enPlayerState_SneakIdle:
		SneakIdleState();
		break;
		//ダメージ状態
	case enPlayerState_Damage:
		DamageState();
		break;
		//ダウン状態
	case enPlayerState_Down:
		DownState();
		break;
	default:
		break;
	}
}
//待機ステート時の処理
void Player::IdleState()
{
	TransitionState();
}
//歩きステート時の処理
void Player::WalkState()
{
	TransitionState();
}
//走りステージの処理
void Player::RunState()
{
	TransitionState();
}
//しゃがみステート時の処理
void Player::SneakState()
{
	TransitionState();
}
//しゃがみ待機ステート時の処理
void Player::SneakIdleState()
{
	TransitionState();
}
//ダメージステート時の処理
void Player::DamageState()
{
	TransitionState();
}
//ダウンステート時の処理
void Player::DownState()
{
	TransitionState();
}

void Player::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}