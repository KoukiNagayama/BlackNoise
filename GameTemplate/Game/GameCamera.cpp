#include "stdafx.h"
#include "sound/SoundEngine.h"
#include "GameCamera.h"
#include "Player.h"
#include "InfoForEdge.h"

namespace
{
	const float CAMPOS_Y = 250.0f;			//視点の高さ
	const float TOCAMPOS_Z = -77.0f;		//注視点までのZ座標

	const float MOVESPEED = 300.0f;			//歩きの移動速度
	const float MOVE_RUN = 1.22f;			//走り時にいくら乗算するか
	const float MOVE_SNEAK = 0.3f;			//しゃがみ時にいくら乗算するか
	const float TARGET_UNDER = -0.99f;		//カメラの下限
	const float TARGET_OVER = 0.35f;		//カメラの上限
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

	//プレイヤーのインスタンスを探す。
	m_player = FindGO<Player>("player");

	//m_position = Vector3::Zero;
	m_modelRender.Init("Assets/modelData/human/playerbox.tkm");
	m_modelRender.SetPosition(m_position);

	//カメラのニアクリップとファークリップを設定する。
	g_camera3D->SetNear(3.0f);
	g_camera3D->SetFar(10000.0f);

	//キャラコンを初期化する。
	m_charaCon.Init(50.0f, 170.0f, m_position);

	//サウンドを登録。
	g_soundEngine->ResistWaveFileBank(3, "Assets/sound/human/walk.wav");
	//サウンドをNewGO
	m_walkSound = NewGO<SoundSource>(3);

	beforeRate = 0.00f;
	g_infoForEdge.SetRate(2, rate);
	g_infoForEdge.InitForSound(2, m_position, 200.0f, 0, rate);

	return true;
}
void GameCamera::Update()
{
	//移動処理
	Move();
	//注視点の処理
	ViewPoint();
	//ステート遷移処理
	ManageState();

	Vector3 position;
	position = m_position;
	position.y = 0.0f;
	m_modelRender.SetPosition(position);
	m_modelRender.Update();

	g_soundEngine->SetListenerPosition(m_position);
	g_soundEngine->SetListenerFront(g_camera3D->GetForward());
	//カメラの更新。
	g_camera3D->Update();
}

void GameCamera::Move()
{
	/*if (m_player->IsEnableMove() == false)
	{
		return;
	}
	}*/
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
	//摩擦
	m_moveSpeed.x -= m_moveSpeed.x * 0.3f;
	m_moveSpeed.z -= m_moveSpeed.z * 0.3f;
	//2ベクトル間の強さが0.001以下だったら
	if (m_moveSpeed.Length() < 0.001f)
	{
		m_moveSpeed.x -= m_moveSpeed.x * 0.3f;
		m_moveSpeed.z -= m_moveSpeed.z * 0.3f;
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
	m_rotation = qRot;

	//カメラの回転の上限をチェックする。
	//注視点から視点までのベクトルを正規化する。
	Vector3 toPosDir = m_toCameraPos;
	toPosDir.Normalize();
	if (toPosDir.y < TARGET_UNDER) {
		//カメラが下向きすぎ。
		m_toCameraPos = toCameraPosOld;
	}
	else if (toPosDir.y > TARGET_OVER) {
		//カメラが上向きすぎ。
		m_toCameraPos = toCameraPosOld;
	}

	//視点を計算する。
	m_target = m_position + m_toCameraPos;
	//メインカメラに注視点と視点を設定する。
	g_camera3D->SetTarget(m_target);
}

void GameCamera::ManageState()
{
	switch (m_moveState)
	{
		//待機ステート
	case enMoveState_Idle:
		IdleState();
		break;
		//歩きステート
	case enMoveState_Walk:
		WalkState();
		break;
		//走りステート
	case enMoveState_Run:
		RunState();
		break;
		//しゃがみステート
	case enMoveState_Sneak:
		SneakState();
		break;
	case enMoveState_SneakIdle:
		SneakIdleState();
		break;
	default:
		break;
	}
}

void GameCamera::IdleState()
{
	TransitionState();
}

void GameCamera::WalkState()
{
	TransitionState();
}

void GameCamera::RunState()
{
	TransitionState();
}

void GameCamera::SneakState()
{
	TransitionState();
}

void GameCamera::SneakIdleState()
{
	TransitionState();
}

void GameCamera::TransitionState()
{

	//xかzの移動速度があったら(スティックの入力があったら)。
	if (fabsf(m_moveSpeed.x) >= 0.01f && fabsf(m_moveSpeed.z) >= 0.01f)
	{
		//Bボタンを押している間は走る。
		if (g_pad[0]->IsPress(enButtonA))
		{
			//走り状態にする。
			m_moveState = enMoveState_Run;
		}
		//LBボタンを押している間しゃがむ。
		if (g_pad[0]->IsPress(enButtonLB2))
		{
			//しゃがみ状態にする。
			m_moveState = enMoveState_Sneak;
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

void GameCamera::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}