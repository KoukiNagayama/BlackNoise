#pragma once
#include "sound/SoundSource.h"
class GameCamera;
class Enemy;

class Door : public IGameObject
{
public:
	//ドアステート。
	enum EnDoorState {
		enDoorState_Open,				//オープン。
		enDoorState_OpenIdle,			//オープンし終わった。
		enDoorState_Close,				//クローズ。
		enDoorState_CloseIdle			//クローズし終わった。
	};
public:
	Door();
	~Door();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	/// <summary>
	/// ステート遷移処理。
	/// </summary>
	void TransitionState();
	/// <summary>
	/// 座標を設定する。
	/// </summary>
	/// <param name="position">座標。</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
	/// 回転を設定する。
	/// </summary>
	/// <param name="rotation">回転。</param>
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}
	/// <summary>
	/// 大きさを設定する。
	/// </summary>
	/// <param name="scale">大きさ。</param>
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}
	/// <summary>
	/// ドアの番号を設定する。
	/// </summary>
	/// <param name="doorNumber">ドアの番号。</param>
	void SetDoorNumber(const int doorNumber)
	{
		m_doorNumber = doorNumber;
	}
private:
	/// <summary>
	/// ドアが開けられる位置にいるか
	/// </summary>
	/// <returns>プレイヤーが近ければtrue</returns>
	bool NearPlayer();
	/// <summary>
	/// エネミーがドアが開けられる位置にいるか
	/// </summary>
	/// <returns>Enemyが近ければtrue</returns>
	bool NearEnemy();
	/// <summary>
	/// アニメーションを再生する。
	/// </summary>
	void PlayAnimation();
	/// <summary>
	/// フィジックスオブジェクトの当たり判定を開放する。
	/// </summary>
	void ReleasePhysicsObject();
	/// <summary>
	/// フィジックスオブジェクトの当たり判定を作成する。
	/// </summary>
	void CreatePhysicsObject();
	/// <summary>
	/// ステート管理。
	/// </summary>
	void ManageState();
	/// <summary>
	/// オープンステートのステート遷移処理。
	/// </summary>
	void OpenState();
	/// <summary>
	/// オープン待機ステートのステート遷移処理。
	/// </summary>
	void OpenIdleState();
	/// <summary>
	/// クローズステートのステート遷移処理。
	/// </summary>
	void CloseState();
	/// <summary>
	/// クローズ待機ステートのステート遷移処理。
	/// </summary>
	void CloseIdleState();
private:
	/// <summary>
	/// 音の生成
	/// </summary>
	/// <param name="number">0->開、1->閉</param>
	void MakeSound(int number);
	/// <summary>
	/// 影響率を調べる
	/// </summary>
	void CheckRate();

	ModelRender					m_modelRender;							//モデルレンダ―。
	Vector3						m_position = Vector3::Zero;				//座標。
	Quaternion					m_rotation = Quaternion::Identity;		//回転。
	Vector3						m_scale = Vector3::One;					//大きさ。
	enum EnAnimationClip {					//アニメーション。
		enAnimationClip_CloseIdle,			//待機。	
		enAnimationClip_OpenIdle,			//オープン。
		enAnimationClip_Close,				//クローズ。
		enAnimationClip_Open,				//オープン
		enAnimationClip_Num,				//アニメーションの数。
	};
	AnimationClip				m_animationClips[enAnimationClip_Num];	//アニメーションクリップ。
	PhysicsStaticObject			m_physicsStaticObject;					//フィジクススタティックオブジェクト。	
	EnDoorState					m_doorState = enDoorState_CloseIdle;	//ドアステート。
	int							m_doorNumber = 0;						//ドアの番号。
	GameCamera*					m_gamecam = nullptr;					//ゲームカメラ。
	float						m_deg = 0.0f;							//ドアの角度
	SoundSource*				m_sound = nullptr;						//開きサウンド
	float						m_rateByTime = 0.0f;
	Enemy*						m_enemy = nullptr;						//エネミー
	bool						m_close = true;							//閉じているか
};

