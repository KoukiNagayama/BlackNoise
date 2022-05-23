#pragma once
#include "sound/SoundSource.h"

class Player;
class Enemy2;
//ゲーム中のカメラを制御する。
class GameCamera : public IGameObject
{
public:
	enum EnMoveState
	{
		enMoveState_Idle,
		enMoveState_Walk,
	};

public:
	GameCamera();
	~GameCamera();

	/// <summary>
	/// 座標の設定。
	/// </summary>
	/// <param name="position">座標。</param>
	void SetPosition(Vector3& position)
	{
		m_position = position;
	}

	void SetRotation(Quaternion& rot)
	{
		m_rotation = rot;
	}
	/// <summary>
	/// 一階の切り替え地点の設定。
	/// </summary>
	/// <param name="position"></param>
	void SetSwitchPosition(Vector3& position)
	{
		m_switchPos = position;
	}
	/// <summary>
	/// カメラの座標の取得
	/// </summary>
	/// <returns>カメラの座標</returns>
	const Vector3 GetPosition()
	{
		return m_position;
	}
	/// <summary>
	/// カメラの高さを0にした座標の取得
	/// </summary>
	/// <returns>カメラの座標</returns>
	const Vector3 GetYaxisZeroPosition()
	{
		Vector3 pos;
		pos = m_position;
		pos.y = 0.0f;
		return pos;
	}
	/// <summary>
	/// 回転の取得。
	/// </summary>
	/// <returns></returns>
	const Quaternion GetRotation()
	{
		return m_rotation;
	}
	/// <summary>
	/// 注視点から視点に向かうベクトルの取得
	/// </summary>
	/// <returns>注視点から視点に向かうベクトル</returns>
	const Vector3 GetToCameraPos()
	{
		return m_toCameraPos;
	}
	/// <summary>
	/// 左スティックが入力中か。
	/// </summary>
	/// <returns>入力中 -> true</returns>
	bool IsStickMove() const
	{
		return m_stickL.x > 0.0f || m_stickL.y > 0.0f;
	}
	/// <summary>
	/// 移動中か。
	/// </summary>
	/// <returns>移動中 -> true</returns>
	bool IsMoveNow() const
	{
		return fabsf(m_moveSpeed.x) >= 0.001f && fabsf(m_moveSpeed.z) >= 0.001f;
	}
	/// <summary>
	/// 足音が鳴っているか
	/// </summary>
	bool IsSound() const
	{
		return m_isSound;
	}
private:
	/////////////////////////////////////
	//メンバ関数
	/////////////////////////////////////

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <returns>true</returns>
	bool Start();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	/// <summary>
	/// ステート遷移処理。
	/// </summary>
	void TransitionState();
	/// <summary>
	/// 移動処理。
	/// </summary>
	void Move();
	/// <summary>
	/// 注視点処理。
	/// </summary>
	void ViewPoint();
	/// <summary>
	/// ステート管理
	/// </summary>
	void ManageState();
	/// <summary>
	/// 待機ステート。
	/// </summary>
	void IdleState();
	/// <summary>
	/// 歩きステート。
	/// </summary>
	void WalkState();
	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="rc">レンダーコンテキスト</param>
	void Render(RenderContext& rc);
	/// <summary>
	/// 影響率を調べる
	/// </summary>
	void CheckRate();
	/// <summary>
	/// 音を鳴らす
	/// </summary>
	void MakeSound();


	/////////////////////////////////////
	//メンバ変数
	/////////////////////////////////////
	Player* m_player = nullptr;										//プレイヤークラスのポインタ。
	Vector3 m_toCameraPos = Vector3::Zero;							//注視点から視点に向かうベクトル。
	Vector3 m_moveSpeed = Vector3::Zero;							//移動速度。
	float m_multiplier = 0.0f;										//移動速度に乗算する値。
	Vector3 m_position = Vector3::Zero;								//座標。
	Vector3 m_stickL = Vector3::Zero;								//左スティック
	Vector3 m_switchPos = Vector3::Zero;							//一階への切り替え地点。
	EnMoveState m_moveState = enMoveState_Idle;						//ステート。
	Vector3 m_target = Vector3::Zero;								//ターゲット。
	CharacterController m_charaCon;									//キャラクターコントローラー。
	Quaternion m_rotation = Quaternion::Identity;					//回転。
	SoundSource* m_sound = nullptr;									//足音
	ModelRender m_modelRender;										//モデル
	Vector3 m_up = Vector3::Zero;									//カメラ上方向
	float m_rateByTime = 0.0f;										//影響率
	Enemy2* m_enemy = nullptr;										//エネミー
	bool m_isSound = false;											//足音が鳴っているか
};
