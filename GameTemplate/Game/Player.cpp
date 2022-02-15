#include "stdafx.h"
#include "Player.h"

namespace
{
	const float MOVESPEED = 250.0f;
}

bool Player::Start()
{
	m_modelRender.Init("Assets/modelData/human/jackie.tkm", animationClips, enAnimationClip_Num);
	m_modelRender.Update();

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
}

void Player::Move()
{
	if (IsEnableMove() == false)
	{
		return;
	}
	//x,zの移動速度を0にする。
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.y = 0.0f;

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
		m_moveSpeed.x *= 1.3f;
		m_moveSpeed.z *= 1.3f;
	}
	//しゃがみステートなら速度を0.7倍にする。
	if (m_playerState == enPlayerState_Sneak)
	{
		m_moveSpeed.x *= 0.7f;
		m_moveSpeed.z *= 0.7f;
	}

	//地面についていたら
	if (m_charaCon.IsOnGround())
	{
		//重力をなくす。
		m_moveSpeed.y = 0.0f;
	}
	//地面についていなかったら
	else
	{
		//重力を発生させる。
		m_moveSpeed.y -= pow(m_moveSpeed.y, 9.8f);
	}

	//キャラコンを使って座標を移動させる。
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	//座標を設定する。
	m_modelRender.SetPosition(m_position);
}

void Player::Rotation()
{
	//移動していないときは回転しない
	if (fabsf(m_moveSpeed.x) < 0.001f
		&& fabsf(m_moveSpeed.z) < 0.001f) {
		return;
	}
	//回転角度を計算する。
	float angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);
	m_rotation.SetRotationY(-angle);

	//回転を設定する。
	m_modelRender.SetRotation(m_rotation);

	//プレイヤーの正面ベクトルを計算する。
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
}