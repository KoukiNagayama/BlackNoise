#pragma once
class Item;
class GameCamera;
class UI;
class Crowbar : public IGameObject
{
public:
	Crowbar() {};
	~Crowbar() {};
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
	/// 座標の設定
	/// </summary>
	/// <param name="position">座標</param>
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
	/// 座標を取得
	/// </summary>
	/// <returns>座標</returns>
	const Vector3 GetPosition()
	{
		return m_position;
	}
	/// <summary>
	/// バールをとれるか
	/// </summary>
	//void CanPickCrowbar();
private:
	Vector3 m_position;						//座標
	Vector3 m_scale;						//拡大率
	Quaternion m_rotation;					//回転

	ModelRender m_modelRender;				//モデル

	Item* m_item = nullptr;					//アイテム
	GameCamera* m_gamecam = nullptr;		//ゲームカメラ
	UI* m_ui = nullptr;						//UI
};

