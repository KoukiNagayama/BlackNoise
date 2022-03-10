#include "stdafx.h"
#include "Player.h"
#include "GameCamera.h"

//定数
namespace
{
	const float MOVESPEED = 1000.0f;
	const float MOVE_RUN = 1.4f;
	const float MOVE_SNEAK = 0.58f;
}

//初期化処理
bool Player::Start()
{

	m_gamecam = FindGO<GameCamera>("gamecamera");

	m_position = m_gamecam->GetPosition();
	m_position.y = 0.0f;

	m_modelRender.Init("Assets/modelData/human/playerNohead.tkm");
	m_modelRender.SetPosition(m_gamecam->GetPosition());

	//キャラコンを初期化する。
	m_charaCon.Init(10.0f, 45.0f, m_position);

	return true;
}

Player::~Player()
{

}

//更新処理
void Player::Update()
{
	//移動処理
	Move();
	//回転処理。
	Rotation();
	//ステート管理。
	ManageState();	

	m_modelRender.Update();
}

void Player::Move()
{
	//FPS視点のため、移動や座標の情報はカメラが保持している。
	//カメラの座標を取得する。
	m_position = m_gamecam->GetPosition();
	//Y座標は0にする。
	m_position.y = 0.0f;
	//座標を設定する。
	m_modelRender.SetPosition(m_position);
}

void Player::Rotation()
{
	//カメラの向きを取得
	Vector3 rotVec = m_gamecam->GetToCameraPos();
	//yの値の情報は要らないので、0.0fを代入して正規化。
	rotVec.y = 0.0f;
	rotVec.Normalize();

	//現在どれだけ回転しているのかの角度を求める。
	float angle = atan2(rotVec.x, rotVec.z);
	//その角度を使って、Y軸回りでQuaternionを設定する。
	m_rotation.SetRotation(Vector3::AxisY, angle);

	//Quaternionを使って、モデルの回転を設定
	m_modelRender.SetRotation(m_rotation);
}

void Player::TransitionState()
{
	//HPが0になったら
	if (m_hp == 0)
	{
		//ダウン状態にする。
		m_playerState = enPlayerState_Down;
		return;
	}
	//xかzの移動速度があったら(スティックの入力があったら)。
	if (fabsf(m_moveSpeed.x) >= 0.001f || fabsf(m_moveSpeed.z) >= 0.001f)
	{
		//Bボタンを押している間は走る。
		if (g_pad[0]->IsPress(enButtonB))
		{
			//走り状態にする。
			m_playerState = enPlayerState_Run;
			return;
		}
		//LBボタンを押している間しゃがむ。
		if (g_pad[0]->IsPress(enButtonLB2))
		{
			//しゃがみ状態にする。
			m_playerState = enPlayerState_Sneak;
			return;
		}
		//歩き状態にする。
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
//ステート管理
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

//描画処理
void Player::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}