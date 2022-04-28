#pragma once
class GameCamera;
class BrokenDoor : public IGameObject
{

public:
	BrokenDoor() {};
	~BrokenDoor() {};
	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <returns>true</returns>
	bool Start();
	/// <summary>
	/// 回転の設定
	/// </summary>
	/// <param name="rotation">クォータニオン</param>
	void SetRotation(Quaternion rotation)
	{
		m_rotation = rotation;
	}
	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(Vector3 position)
	{
		m_position = position;
	}
	/// <summary>
	/// ハンマーを拾った
	/// </summary>
	void PickHammer()
	{
		m_haveHammer = true;
	}
private:
	enum EnBreakDoorState
	{
		enDoorState_Before,			//ドアを壊す前
		enDoorState_CloseIdle,		//クローズ待機
		enDoorState_Close,			//クローズ
		enDoorState_OpenIdle,		//オープン待機
		enDoorState_Open			//オープン
	};
private:
	/////関数/////
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	/// <summary>
	/// ステート管理処理
	/// </summary>
	void ManageState();
	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="rc">レンダーコンテキスト</param>
	void Render(RenderContext& rc);
	/// <summary>
	/// フィジックスオブジェクトの生成
	/// </summary>
	void CreatePhysicsObject();
	/// <summary>
	/// フィジックスオブジェクトの開放
	/// </summary>
	void ReleasePhysicsObject();
	/// <summary>
	/// ステート遷移処理
	/// </summary>
	void TransitionState();
	/// <summary>
	/// 壊す前ステート
	/// </summary>
	void BeforeState();
	/// <summary>
	/// クローズ待機ステート
	/// </summary>
	void CloseIdleState();
	/// <summary>
	/// クローズステート
	/// </summary>
	void CloseState();
	/// <summary>
	/// オープンステート
	/// </summary>
	void OpenState();
	/// <summary>
	/// オープン待機ステート
	/// </summary>
	void OpenIdleState();
	/// <summary>
	/// アニメーションの再生
	/// </summary>
	void PlayAnimation();
	/// <summary>
	/// ドアに近いか
	/// </summary>
	/// <returns>近いとき->true</returns>
	bool NearDoor();
	/// <summary>
	/// ドアを壊せるか
	/// </summary>
	bool CanBreakDoor();

	/////変数/////
	enum EnAnimationClip {					//アニメーション。
		enAnimationClip_Before,				//壊す前。
		enAnimationClip_CloseIdle,			//待機。	
		enAnimationClip_OpenIdle,			//オープン。
		enAnimationClip_Close,				//クローズ。
		enAnimationClip_Open,				//オープン
		enAnimationClip_Num,				//アニメーションの数。
	};
	AnimationClip				m_animationClips[enAnimationClip_Num];	//アニメーションクリップ。
	ModelRender					m_modelRender;							//モデル
	Quaternion					m_rotation;								//回転
	PhysicsStaticObject			m_physicsStaticObject;					//フィジクススタティックオブジェクト。	
	Vector3						m_position = Vector3::Zero;								//座標
	EnBreakDoorState			m_doorState = enDoorState_Before;		//ステート
	GameCamera*					m_gamecam = nullptr;					//ゲームカメラ
	bool						m_haveHammer = false;					//ハンマーを持っているか
};

