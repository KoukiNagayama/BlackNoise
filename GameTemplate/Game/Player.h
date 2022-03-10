#pragma once

class GameCamera;

class Player : public IGameObject
{
public:
	/// <summary>
	/// ステート
	/// </summary>
	enum EnPlayerState {
		enPlayerState_Idle,			//待機ステート
		enPlayerState_Walk,			//歩きステート
		enPlayerState_Run,			//走りステート
		enPlayerState_Sneak,		//しゃがみ、隠れステート
		enPlayerState_SneakIdle,	//しゃがみ、隠れ待機ステート
		enPlayerState_Damage,		//ダメージステート
		enPlayerState_Down			//ダウンステート
	};
public:
	Player() {};
	~Player();
	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <returns>true</returns>
	bool Start();
	/// <summary>
	/// 移動処理の制限。
	/// </summary>
	/// <returns></returns>
	bool IsEnableMove() const
	{
		return m_playerState != enPlayerState_Damage &&
			m_playerState != enPlayerState_Down;
	}
	/// <summary>
	/// ダメージステートの設定
	/// </summary>
	void ReceiveDamege()
	{
		m_playerState = enPlayerState_Damage;
	}
	/// <summary>
	/// 座標を渡す。
	/// </summary>
	/// <returns>m_positon</returns>
	const Vector3 GetPosition()
	{
		return m_position;
	}

private:
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();
	/// <summary>
	/// 回転処理
	/// </summary>
	void Rotation();
	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="rc">レンダーコンテキスト</param>
	void Render(RenderContext& rc);
	/// <summary>
	/// ステート遷移処理。
	/// </summary>
	void TransitionState();
	/// <summary>
	/// ステート管理。
	/// </summary>
	void ManageState();
	/// <summary>
	/// 待機ステート時の処理。
	/// </summary>
	void IdleState();
	/// <summary>
	/// 歩きステート時の処理。
	/// </summary>
	void WalkState();
	/// <summary>
	/// 走りステート時の処理。
	/// </summary>
	void RunState();
	/// <summary>
	/// しゃがみ・隠れステート時の処理。
	/// </summary>
	void SneakState();
	/// <summary>
	/// しゃがみ待機ステート時の処理。
	/// </summary>
	void SneakIdleState();
	/// <summary>
	/// ダメージステート時の処理。
	/// </summary>
	void DamageState();
	/// <summary>
	/// ダウンステート時の処理。
	/// </summary>
	void DownState();

	//メンバ変数
	//アニメーションクリップ
	enum EnAnimationClip
	{
		enAnimationClip_Idle,			//待機アニメーション
		enAnimationClip_Walk,			//歩きアニメーション
		enAnimationClip_Run,			//走りアニメーション
		enAnimationClip_Sneak,			//しゃがみ・隠れアニメーション
		enAnimationClip_SneakIdle,		//しゃがみ・隠れアイドルアニメーション
		enAnimationClip_Damage,			//ダメージアニメーション
		enAnimationClip_Down,			//ダウンアニメーション
		enAnimationClip_Num				//アニメーションの数
	};

	int m_hp = 3;											//HP

	Vector3 m_moveSpeed;									//移動速度
	Vector3 m_toCameraPos;									//カメラの注視点から視点に向かうベクトル
	Vector3 m_position ;									//座標
	Quaternion m_rotation = Quaternion::Identity;			//回転
	EnPlayerState m_playerState = enPlayerState_Idle;		//ステート
	ModelRender m_modelRender;								//モデルレンダー
	AnimationClip animationClips[enAnimationClip_Num];		//アニメーションクリップ
	CharacterController m_charaCon;							//キャラクターコントローラー
	GameCamera* m_gamecam = nullptr;						//ゲームカメラのポインタ
};

