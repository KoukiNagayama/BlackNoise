#pragma once
#include "sound/SoundSource.h"
class Item;
class GameCamera;
class FloorGimmick : public IGameObject
{
public:
	FloorGimmick() {};
	~FloorGimmick();
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
	/// 描画処理
	/// </summary>
	/// <param name="rc">レンダーコンテキスト</param>
	void Render(RenderContext& rc);
	/// <summary>
	/// ドアを壊せる距離にいるか
	/// </summary>
	void NearGimmick();
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
	/// 攻撃回数を取得
	/// </summary>
	/// <returns></returns>
	int GetAttackCount() const 
	{
		return m_attack;
	}
	/// <summary>
	/// 座標を取得
	/// </summary>
	/// <returns>座標</returns>
	const Vector3 GetPosition()
	{
		return m_position;
	}
private:
	/// <summary>
	/// フィジックスオブジェクトの生成
	/// </summary>
	void CreatePhysicsObject();
	/// <summary>
	/// フィジックスオブジェクトの開放
	/// </summary>
	void ReleasePhysicsObject();
	/// <summary>
/// 破壊音の生成
/// </summary>
	void MakeSound();
	/// <summary>
	/// 影響率を調べる
	/// </summary>
	void CheckRate();

	Vector3 m_position;	//座標
	Vector3 m_scale;	//拡大率
	Quaternion m_rotation;	//回転
	ModelRender m_modelRender;	//モデル

	PhysicsStaticObject	m_physicsStaticObject;	//フィジクススタティックオブジェクト。	
	int m_attack = 2;	//壊れるまでに必要な攻撃回数

	Item* m_item = nullptr;	//アイテムクラス
	GameCamera* m_gamecam = nullptr;	//ゲームカメラ
	SoundSource* m_sound = nullptr;		//破壊音
	float m_rateByTime = 0.0f;

};

