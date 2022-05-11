#pragma once
class TitleText : public IGameObject
{
public:
	TitleText() {};
	~TitleText() {};
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
	/// 座標を取得
	/// </summary>
	/// <returns>座標</returns>
	const Vector3 GetPosition() const
	{
		return m_position;
	}
private:
	ModelRender			m_modelRender;			// モデルレンダー
	Vector3				m_position;				// 座標
	Vector3				m_scale;				// 拡大率
	Quaternion			m_rotation;				// 回転
};

