#include "stdafx.h"
#include "sound/SoundEngine.h"
#include "GameCamera.h"
#include "Player.h"
#include "InfoForEdge.h"
#include "Enemy2.h"

namespace
{
	const float CAMPOS_Y = 250.0f;			//視点の高さ
	const float TOCAMPOS_Z = -77.0f;		//注視点までのZ座標
	const float DISTANCE = 100.0f;			//プレイヤーとの距離	
	const float MOVESPEED = 500.0f;			//歩きの移動速度
	const float MOVE_RUN = 1.22f;			//走り時にいくら乗算するか
	const float MOVE_SNEAK = 0.3f;			//しゃがみ時にいくら乗算するか
	const float TARGET_UNDER = -0.7f;		//カメラの下限
	const float TARGET_OVER = 0.35f;		//カメラの上限
	const float EDGE_FADE_IN_DELTA_VALUE = 0.05f;	// エッジがフェードインするときの変位量
	const float EDGE_FADE_OUT_DELTA_VALUE = 0.05f;	// エッジがフェードアウトするときの変位量
	const float RATE_BY_TIME_MAX_VALUE = 1.00f;		// 時間による影響率の最大値
	const float RATE_BY_TIME_MIN_VALUE = 0.00f;		// 時間による影響率の最小値
	const float SOUND_RANGE = 180.0f;				//影響範囲
	const float VOLUME = 0.7f;						//ボリューム
	const float FRICTION = 0.5f;					//摩擦
	const float VIEWPOINT_UP = 350.0f;				//敵の顔の高さ
}

GameCamera::GameCamera()
{
}
GameCamera::~GameCamera()
{
	g_infoForEdge.SetIsSound(6, 0);
	g_infoForEdge.SetRate(6, 0.00f);
	DeleteGO(m_sound);
}
bool GameCamera::Start()
{
	//注視点から視点までのベクトルを設定。
	m_toCameraPos.Set(0.0f, 0.0f, TOCAMPOS_Z);

	//プレイヤーのインスタンスを探す。
	m_player = FindGO<Player>("player");
	//エネミー
	m_enemy = FindGO<Enemy2>("enemy");

	//m_position = Vector3::Zero;
	m_modelRender.Init("Assets/modelData/human/playerbox.tkm");
	m_modelRender.SetPosition(m_position);

	//カメラのニアクリップとファークリップを設定する。
	g_camera3D->SetNear(3.0f);
	g_camera3D->SetFar(10000.0f);

	//キャラコンを初期化する。
	m_charaCon.Init(50.0f, 170.0f, m_position);

	//サウンドを登録。
	g_soundEngine->ResistWaveFileBank(11, "Assets/sound/human/walk.wav");
	//サウンドをNewGO
	m_sound = NewGO<SoundSource>(11);
	m_sound->Init(11);
	m_sound->SetVolume(VOLUME);

	g_infoForEdge.InitForSound(6, m_position, 200.0f, 0, m_rateByTime);

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
	//影響率を調べる
	CheckRate();

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
	////x,zの移動速度を0にする。
	//m_moveSpeed.x = 0.0f;
	//m_moveSpeed.z = 0.0f;


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

	//摩擦
	m_moveSpeed.x -= m_moveSpeed.x * FRICTION;
	m_moveSpeed.z -= m_moveSpeed.z * FRICTION;
	//2ベクトル間の強さが0.001以下だったら
	if (m_moveSpeed.Length() < 0.001f)
	{
		m_moveSpeed.x -= m_moveSpeed.x * FRICTION;
		m_moveSpeed.z -= m_moveSpeed.z * FRICTION;
	}
	
	//キャラコンを使って座標を移動させる。
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	//視点の高さを上げる。
	m_position.y = CAMPOS_Y;
	//カメラの座標を設定する。
	g_camera3D->SetPosition(m_position);
}
//注視点の処理
void GameCamera::ViewPoint()
{
	//カメラを更新。
	//注視点を計算する。
	//m_target = m_position;
	//プレイヤの足元からちょっと上を注視点とする。
	//m_target += g_camera3D->GetForward() * 50.0f;

	Vector3 toCameraPosOld = m_toCameraPos;
	//パッドの入力を使ってカメラを回す。
	float x = g_pad[0]->GetRStickXF() * 1.5f;
	float y = g_pad[0]->GetRStickYF() * 1.5f;



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

	//プレイヤーが攻撃を受けたら
	/*if (m_player->IsEnableMove() == false) {
		m_target = m_enemy->GetPosition();
		m_target.y += VIEWPOINT_UP;
	}*/
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
	default:
		break;
	}
}

void GameCamera::IdleState()
{
	m_sound->Pause();
	m_isSound = false;
	TransitionState();
}

void GameCamera::WalkState()
{
	m_sound->Play(true);
	m_isSound = true;
	TransitionState();
}

void GameCamera::TransitionState()
{

	//xかzの移動速度があったら(スティックの入力があったら)。
	if (fabsf(m_moveSpeed.x) >= 0.01f && fabsf(m_moveSpeed.z) >= 0.01f)
	{
		//歩き状態にする。
		m_moveState = enMoveState_Walk;
		return;
	}
	m_moveState = enMoveState_Idle;
}

void GameCamera::CheckRate()
{
	int check1;
	if (m_sound != nullptr) {
		if (m_moveState == enMoveState_Walk)
		{
			check1 = 1;
			if (m_rateByTime < RATE_BY_TIME_MAX_VALUE) {
				m_rateByTime += EDGE_FADE_IN_DELTA_VALUE;
			}
		}
		else {
			check1 = 0;
			if (m_rateByTime > RATE_BY_TIME_MIN_VALUE && check1 == 0) {
				m_rateByTime -= EDGE_FADE_OUT_DELTA_VALUE;
				if (m_rateByTime <= RATE_BY_TIME_MIN_VALUE) {
					m_rateByTime = RATE_BY_TIME_MIN_VALUE;
				}
			}
		}
		g_infoForEdge.SetInfoForSound(6, GetYaxisZeroPosition(), SOUND_RANGE, check1, m_rateByTime);

	}
}

void GameCamera::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}