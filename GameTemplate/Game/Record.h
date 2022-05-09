#pragma once
class Gramophone;
class GameCamera;
class Item;

class Record : public IGameObject
{
public:
	Record() {};
	~Record() {};
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	/// <summary>
	/// 座標を設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
	/// 拡大率を設定
	/// </summary>
	/// <param name="scale">拡大率</param>
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}
	/// <summary>
	/// 回転を設定
	/// </summary>
	/// <param name="rotation">回転</param>
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}
	/// <summary>
	/// オブジェクトの番号を設定
	/// </summary>
	/// <param name="number">オブジェクトの番号</param>
	void SetNumber(const int& number)
	{
		m_number = number;
	}
private:
	Vector3					m_position;							// 座標
	Vector3					m_scale = Vector3::Zero;			// 拡大率
	Quaternion				m_rotation = Quaternion::Identity;	// 回転
	ModelRender				m_modelRender;						// モデルレンダー
	int						m_number;
	GameCamera*				m_gameCamera;
	Vector3					m_gameCameraPos;
	Item*					m_item;

};

