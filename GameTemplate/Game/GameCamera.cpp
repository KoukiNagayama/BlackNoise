#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"

namespace
{
	const float CAMPOS_Y = 170.0f;	//視点の高さ
	const float TOCAMPOS_Z = -77.0f;			//注視点までのZ座標

	const float MOVESPEED = 140.0f;			//歩きの移動速度
	const float MOVE_RUN = 10.8f;			//走り時にいくら乗算するか
	const float MOVE_SNEAK = 0.3f;			//しゃがみ時にいくら乗算するか
	const float MOVE_FRICTION = 30.0f;
}

GameCamera::GameCamera()
{
}
GameCamera::~GameCamera()
{
}
bool GameCamera::Start()
{
	//注視点から視点までのベクトルを設定。
	m_toCameraPos.Set(0.0f, 0.0f, TOCAMPOS_Z);
	
	////////////////////////デバッグカメラ//////////////////////
	//m_toCameraPos.Set(0.0f, CAMPOS_TARGET_Y, -250.0f);
	////////////////////////デバッグカメラ//////////////////////

	//プレイヤーのインスタンスを探す。
	m_player = FindGO<Player>("player");

	m_position = Vector3::Zero;

	//カメラのニアクリップとファークリップを設定する。
	g_camera3D->SetNear(3.0f);
	g_camera3D->SetFar(10000.0f);

	//キャラコンを初期化する。
	m_charaCon.Init(1.0f, 1.0f, m_position);

	return true;
}
void GameCamera::Update()
{
	//移動処理
	Move();
	//注視点の処理
	ViewPoint();
	//ステート遷移処理
	TransitionState();

	//カメラの更新。
	g_camera3D->Update();
}

void GameCamera::Move()
{
	if (m_player->IsEnableMove() == false)
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
	if (m_moveState == enMoveState_Run)
	{
		m_moveSpeed.x *= MOVE_RUN;
		m_moveSpeed.z *= MOVE_RUN;
	}
	//しゃがみステートなら速度を0.7倍にする。
	if (m_moveState == enMoveState_Sneak)
	{
		m_moveSpeed.x *= MOVE_SNEAK;
		m_moveSpeed.z *= MOVE_SNEAK;
	}
	
	//キャラコンを使って座標を移動させる。
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	//視点の高さを上げる。
	m_position.y = CAMPOS_Y;
	//しゃがみ状態だったら
	if (IsSneak() == false)
	{
		m_position.y -= m_position.y / 2;
	}
	//カメラの座標を設定する。
	g_camera3D->SetPosition(m_position);
}
//注視点の処理
void GameCamera::ViewPoint()
{
	//カメラを更新。
	//注視点を計算する。
	m_target = m_position;
	//プレイヤの足元からちょっと上を注視点とする。
	m_target += g_camera3D->GetForward() * 50.0f;

	Vector3 toCameraPosOld = m_toCameraPos;
	//パッドの入力を使ってカメラを回す。
	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();
	//Y軸周りの回転
	Quaternion qRot;
	qRot.SetRotationDeg(Vector3::AxisY, 1.3f * x);
	qRot.Apply(m_toCameraPos);

	//X軸周りの回転。
	Vector3 axisX;
	axisX.Cross(Vector3::AxisY, m_toCameraPos);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX, 1.3f * -y);
	qRot.Apply(m_toCameraPos);

	//カメラの回転の上限をチェックする。
	//注視点から視点までのベクトルを正規化する。
	Vector3 toPosDir = m_toCameraPos;
	toPosDir.Normalize();
	if (toPosDir.y < -0.99f) {
		//カメラが上向きすぎ。
		m_toCameraPos = toCameraPosOld;
	}
	else if (toPosDir.y > 0.9f) {
		//カメラが下向きすぎ。
		m_toCameraPos = toCameraPosOld;
	}

	//視点を計算する。
	m_target = m_position + m_toCameraPos;
	//メインカメラに注視点と視点を設定する。
	g_camera3D->SetTarget(m_target);
}

void GameCamera::TransitionState()
{

	//xかzの移動速度があったら(スティックの入力があったら)。
	if (IsMoveNow()==true)
	{
		//Bボタンを押している間は走る。
		if (g_pad[0]->IsPress(enButtonB))
		{
			//走り状態にする。
			m_moveState = enMoveState_Run;
			return;
		}
		//LBボタンを押している間しゃがむ。
		if (g_pad[0]->IsPress(enButtonLB2))
		{
			//しゃがみ状態にする。
			m_moveState = enMoveState_Sneak;
			return;
		}
		//歩き状態にする。
		m_moveState = enMoveState_Walk;
	}
	else
	{
		//LB2を押していたら
		if (g_pad[0]->IsPress(enButtonLB2))
		{
			//しゃがみ待機状態にする。
			m_moveState = enMoveState_SneakIdle;
			return;
		}
		//待機状態にする。
		m_moveState = enMoveState_Idle;
	}
}