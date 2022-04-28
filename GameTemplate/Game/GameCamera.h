#pragma once
#include "sound/SoundSource.h"

class Player;
//ゲーム中のカメラを制御する。
class GameCamera : public IGameObject
{
public:
	enum EnMoveState
	{
		enMoveState_Idle,
		enMoveState_Walk,
		enMoveState_Run,
		enMoveState_Sneak,
		enMoveState_SneakIdle
	};

public:
	GameCamera();
	~GameCamera();

	/// <summary>
	/// 現在のステートがしゃがみ状態でないか
	/// </summary>
	/// <returns>しゃがみ状態の時はfalse</returns>
	bool IsSneak() const
	{
		return m_moveState != enMoveState_Sneak &&
			m_moveState != enMoveState_SneakIdle;
	}
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
	/// 走りステート。
	/// </summary>
	void RunState();
	/// <summary>
	/// しゃがみステート。
	/// </summary>
	void SneakState();
	/// <summary>
	/// しゃがみ待機ステート。
	/// </summary>
	void SneakIdleState();
	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="rc">レンダーコンテキスト</param>
	void Render(RenderContext& rc);
	/// <summary>
	/// アニメーションイベント
	/// </summary>
	/// <param name="clipName">アニメーションの名前</param>
	/// <param name="eventName">アニメーションキーの名前</param>
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);


	/////////////////////////////////////
	//メンバ変数
	/////////////////////////////////////
	Player* m_player = nullptr;						//プレイヤークラスのポインタ。
	Vector3 m_toCameraPos;							//注視点から視点に向かうベクトル。
	Vector3 m_moveSpeed;							//移動速度。
	float m_multiplier;								//移動速度に乗算する値。
	Vector3 m_position;								//座標。
	Vector3 m_stickL;								//左スティック
	Vector3 m_switchPos;							//一階への切り替え地点。
	EnMoveState m_moveState = enMoveState_Idle;		//ステート。
	Vector3 m_target;								//ターゲット。
	CharacterController m_charaCon;					//キャラクターコントローラー。
	Quaternion m_rotation;							//回転。
	SoundSource* m_walkSound;
	ModelRender m_modelRender;
	Vector3 m_up;
	float rate = 0.0f;
	float beforeRate;
};
