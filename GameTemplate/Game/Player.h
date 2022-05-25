#pragma once

class GameCamera;

class Player : public IGameObject
{
public:
	/// <summary>
	/// ステート
	/// </summary>
	enum EnPlayerState {
		enPlayerState_Nomal,		//通常
		enPlayerState_Damage,		//ダメージステート
		enPlayerState_Stop			//ダウンステート
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
			m_playerState != enPlayerState_Stop;
	}
	/// <summary>
	/// ダメージステートの設定
	/// </summary>
	void ReceiveDamege()
	{
		m_playerState = enPlayerState_Damage;
	}

	void SetStopState()
	{
		m_playerState = enPlayerState_Stop;
	}

	void SetNormalState()
	{
		m_playerState = enPlayerState_Nomal;
	}
	/// <summary>
	/// 座標を渡す。
	/// </summary>
	/// <returns>m_positon</returns>
	const Vector3 GetPosition()
	{
		return m_position;
	}

	const Quaternion GetRotation()
	{
		return m_rotation;
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
	/// 通常時の処理。
	/// </summary>
	void NomalState();
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
	EnPlayerState m_playerState = enPlayerState_Nomal;		//ステート
	ModelRender m_modelRender;								//モデルレンダー
	AnimationClip animationClips[enAnimationClip_Num];		//アニメーションクリップ
	GameCamera* m_gamecam = nullptr;						//ゲームカメラのポインタ
};

