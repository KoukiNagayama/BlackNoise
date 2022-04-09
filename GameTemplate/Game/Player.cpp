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

	

	Vector3 position;
	position = m_position;
	position.y = 0.0f;
	//m_modelRender.Init("Assets/modelData/human/playerbox.tkm");
	//m_modelRender.SetPosition(m_position);
	//キャラコンを初期化する。
	//m_charaCon.Init(10.0f, 45.0f, position);

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
}
//ステート管理
void Player::ManageState()
{
	switch (m_playerState)
	{
		//ノーハンドステート
	case enPlayerState_Nomal:
		NomalState();
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
//
void Player::NomalState()
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