#pragma once
class Gramophone;
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
private:
	Vector3					m_position;				// 座標
	Vector3					m_scale;		// 拡大率
	Quaternion				m_rotation;				// 回転
	ModelRender				m_modelRender;			// モデルレンダー

};

